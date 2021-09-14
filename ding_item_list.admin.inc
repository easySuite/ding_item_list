<?php

/**
 * @file
 * Ding Item List administration functionality.
 */

/**
 * Settings form.
 */
function ding_item_list_settings_form($form, &$form_state) {
  $options = [];

  // Get all modules which implements "hook_ding_entity_buttons".
  $modules = module_implements('ding_entity_buttons');
  foreach ($modules as $module) {
    $options[$module] = $module;
  }

  $form['ding_item_list'] = [
    '#type' => 'fieldset',
    '#title' => t('Displayed buttons'),
    '#description' => t('Permits control of which buttons are displayed on Ding Item List items.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  ];

  $form['ding_item_list']['ding_item_list_button_modules'] = [
    '#type' => 'checkboxes',
    '#title' => t('Modules'),
    '#description' => t('Modules which implements "hook_ding_entity_buttons".'),
    '#options' => $options,
    '#default_value' => variable_get('ding_item_list_button_modules', []),
  ];

  return system_settings_form($form);
}
