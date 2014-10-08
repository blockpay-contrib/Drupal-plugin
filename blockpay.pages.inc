<?php

/**
 * Page callback for BlockPay notification URL.
 */
function blockpay_callback() {
  blockpay_initialize();
  $invoice = bpVerifyNotification();
  if (variable_get('blockpay_debug', FALSE)) {
    watchdog('debug', 'Receiving BlockPay invoice status update: %invoice.', array('%invoice' => print_r($invoice, TRUE)), WATCHDOG_DEBUG);
  }
  if (!is_array($invoice)) {
    watchdog('blockpay', 'Error while processing BlockPay invoice update notification: %message.', array('%message' => strval($invoice)), WATCHDOG_ERROR);
    return MENU_ACCESS_DENIED;
  }
  // Update the record for this invoice.
  $record['btc_price'] = $invoice['btcPrice'];
  $record['status'] = $invoice['status'];
  blockpay_write_record($record, $invoice['id']);
  module_invoke_all('blockpay_invoice_update', $invoice);
}
