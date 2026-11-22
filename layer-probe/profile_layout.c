#include <stdint.h>

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/settings/settings.h>

#include <zmk/ble.h>
#include <zmk/event_manager.h>
#include <zmk/events/ble_active_profile_changed.h>
#include <zmk/events/layer_state_changed.h>
#include <zmk/keymap.h>

#include "../config/boards/shields/LiNEA40/20261027_layers.h"

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#define MAC_PROFILE 1
#define IPAD_PROFILE 4

// -1 until Zephyr finishes reading the saved Bluetooth profile at boot.
static int saved_profile = -1;

static uint8_t profile_layer(int profile) {
    switch (profile) {
    case MAC_PROFILE:
        return L_MAC_BASE;
    case IPAD_PROFILE:
        return L_IPAD_BASE;
    default:
        return L_BASE;
    }
}

static void update_profile_layout(struct k_work *work) {
    const uint8_t profile = zmk_ble_active_profile_index();
    const uint8_t layer = profile_layer(profile);

    if (layer != L_MAC_BASE) {
        zmk_keymap_layer_deactivate(L_MAC_BASE);
    }
    if (layer != L_IPAD_BASE) {
        zmk_keymap_layer_deactivate(L_IPAD_BASE);
    }
    zmk_keymap_layer_activate(layer);

    if (profile != saved_profile) {
        // Persist explicit profile changes without waiting for ZMK's settings debounce.
        const int err = settings_save_one("ble/active_profile", &profile, sizeof(profile));
        if (err < 0) {
            LOG_WRN("Could not save the active LiNEA40 profile (%d)", err);
        } else {
            saved_profile = profile;
        }
    }
}

K_WORK_DEFINE(profile_layout_work, update_profile_layout);

static int restore_profile_layout(void) {
    saved_profile = zmk_ble_active_profile_index();
    k_work_submit(&profile_layout_work);
    return 0;
}

// Schedule the initial layer update outside the settings load callback.
SETTINGS_STATIC_HANDLER_DEFINE(linea40_layout, "linea40_layout", NULL, NULL, restore_profile_layout,
                               NULL);

static int profile_layout_listener(const zmk_event_t *event) {
    if (saved_profile < 0) {
        return ZMK_EV_EVENT_BUBBLE;
    }

    if (as_zmk_ble_active_profile_changed(event) != NULL) {
        k_work_submit(&profile_layout_work);
    } else {
        const struct zmk_layer_state_changed *changed = as_zmk_layer_state_changed(event);
        // &to clears overlays. Restore the selected one after the layer transition.
        if (changed && !changed->state &&
            changed->layer == profile_layer(zmk_ble_active_profile_index())) {
            k_work_submit(&profile_layout_work);
        }
    }

    return ZMK_EV_EVENT_BUBBLE;
}

ZMK_LISTENER(profile_layout, profile_layout_listener);
ZMK_SUBSCRIPTION(profile_layout, zmk_ble_active_profile_changed);
ZMK_SUBSCRIPTION(profile_layout, zmk_layer_state_changed);
