#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/kernel.h>
#include <zmk/ble.h>
#include <zmk/event_manager.h>
#include <zmk/events/layer_state_changed.h>
#include <zmk/keymap.h>

#define SERVICE_UUID BT_UUID_128_ENCODE(0x48cde4f1, 0xf365, 0x4d41, 0x9c39, 0x75699c509e5c)
#define LAYER_UUID   BT_UUID_128_ENCODE(0x48cde4f2, 0xf365, 0x4d41, 0x9c39, 0x75699c509e5c)

static ssize_t read_layer(struct bt_conn *conn, const struct bt_gatt_attr *attr,
                          void *buf, uint16_t len, uint16_t offset) {
    const uint8_t layer = zmk_keymap_highest_layer_active();
    return bt_gatt_attr_read(conn, attr, buf, len, offset, &layer, sizeof(layer));
}

BT_GATT_SERVICE_DEFINE(layer_probe,
    BT_GATT_PRIMARY_SERVICE(BT_UUID_DECLARE_128(SERVICE_UUID)),
    BT_GATT_CHARACTERISTIC(BT_UUID_DECLARE_128(LAYER_UUID),
                           BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
                           BT_GATT_PERM_READ_ENCRYPT, read_layer, NULL, NULL),
    BT_GATT_CCC(NULL, BT_GATT_PERM_READ_ENCRYPT | BT_GATT_PERM_WRITE_ENCRYPT));

static void send_layer(struct k_work *work) {
    ARG_UNUSED(work);
    struct bt_conn *conn = zmk_ble_active_profile_conn();
    if (!conn) {
        return;
    }

    /* Index 2 is the characteristic value, after the service and declaration. */
    const struct bt_gatt_attr *value = &layer_probe.attrs[2];
    if (bt_gatt_is_subscribed(conn, value, BT_GATT_CCC_NOTIFY)) {
        const uint8_t layer = zmk_keymap_highest_layer_active();
        bt_gatt_notify(conn, value, &layer, sizeof(layer));
    }
    bt_conn_unref(conn);
}

K_WORK_DEFINE(layer_work, send_layer);

static int layer_changed(const zmk_event_t *event) {
    ARG_UNUSED(event);
    k_work_submit(&layer_work);
    return ZMK_EV_EVENT_BUBBLE;
}

ZMK_LISTENER(layer_probe_listener, layer_changed);
ZMK_SUBSCRIPTION(layer_probe_listener, zmk_layer_state_changed);
