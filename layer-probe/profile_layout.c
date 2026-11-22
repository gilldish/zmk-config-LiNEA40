#include <stdint.h>

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

static uint8_t selected_profile;

static uint8_t profile_layer(void) {
    switch (selected_profile) {
    case MAC_PROFILE:
        return L_MAC_BASE;
    case IPAD_PROFILE:
        return L_IPAD_BASE;
    default:
        return L_BASE;
    }
}

static void apply_profile_layout(void) {
    const uint8_t layer = profile_layer();

    if (layer != L_MAC_BASE) {
        zmk_keymap_layer_deactivate(L_MAC_BASE);
    }
    if (layer != L_IPAD_BASE) {
        zmk_keymap_layer_deactivate(L_IPAD_BASE);
    }
    zmk_keymap_layer_activate(layer);
}

static int restore_profile_layout(void) {
    selected_profile = zmk_ble_active_profile_index();
    apply_profile_layout();
    return 0;
}

// ZMK restores ble/active_profile during settings load. Apply it after all values are read.
SETTINGS_STATIC_HANDLER_DEFINE(linea40_layout, "linea40_layout", NULL, NULL, restore_profile_layout,
                               NULL);

static int profile_layout_listener(const zmk_event_t *event) {
    if (as_zmk_ble_active_profile_changed(event) != NULL) {
        const uint8_t profile = zmk_ble_active_profile_index();

        if (profile != selected_profile) {
            selected_profile = profile;
            apply_profile_layout();

            // Persist immediately so a power cycle need not wait for ZMK's settings debounce.
            const int err = settings_save_one("ble/active_profile", &profile, sizeof(profile));
            if (err < 0) {
                LOG_WRN("Could not save the active LiNEA40 profile (%d)", err);
            }
        }
    } else {
        const struct zmk_layer_state_changed *changed = as_zmk_layer_state_changed(event);
        const uint8_t layer = profile_layer();

        // &to clears layers in descending order. Restore the selected base overlay as it clears it.
        if (changed != NULL && !changed->state && changed->layer == layer) {
            zmk_keymap_layer_activate(layer);
        }
    }

    return ZMK_EV_EVENT_BUBBLE;
}

ZMK_LISTENER(profile_layout, profile_layout_listener);
ZMK_SUBSCRIPTION(profile_layout, zmk_ble_active_profile_changed);
ZMK_SUBSCRIPTION(profile_layout, zmk_layer_state_changed);
