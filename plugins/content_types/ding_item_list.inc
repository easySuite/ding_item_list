<?php
/**
 * @file
 * CTools plugin definition file.
 */

/**
 * General ctools plugin settings.
 */
$plugin = array(
  'title' => t('Ding item list'),
  'category' => t('Ding!'),
  'description' => t('Add ting items as a list'),
  'single' => TRUE,
  'edit form' => 'ding_nodelist_content_type_edit_form',
  'render callback' => 'ding_item_list_plugin_render',
  'edit form' => 'ding_item_list_plugin_edit_form'
);

/**
 * Plugin content renderer.
 */
function ding_item_list_plugin_render($subtype, $conf, $panel_args, $context) {
  $plugin_data = new stdClass();
  $plugin_data->content = '';

  $item_query = $conf['item_query'];
  $item_count = $conf['item_count'];

  $objects = ding_item_list_get_objects($item_query, $item_count);

  $markup = '';
  foreach ($objects as $object) {
    $markup .= theme('ding_item_list_item', array(
      'cover' => file_create_url($object->image),
      'title' => $object->title,
      'author' => $object->creator,
      'year' => $object->year,
      'rating' => ding_item_list_build_rating($object->rating),
      'rating_count' => $object->rating_count,
      'review_count' => $object->comment_count,
    ));
  }

  $plugin_data->content = theme('ding_item_list_list', array('items' => $markup));

  return $plugin_data;
}

/**
 * Plugin edit form.
 */
function ding_item_list_plugin_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];

  $form['plugin_settings']['list'] = array(
    '#type' => 'fieldset',
    '#title' => t('List settings'),
  );

  $form['plugin_settings']['list']['item_query'] = array(
    '#type' => 'textfield',
    '#title' => t('Ting query string'),
    '#default_value' => $conf['item_query'],
    '#required' => TRUE,
  );

  $form['plugin_settings']['list']['item_count'] = array(
    '#type' => 'textfield',
    '#title' => t('Items count'),
    '#default_value' => isset($conf['item_count']) ? $conf['item_count'] : 3,
    '#required' => TRUE,
    '#size' => 3,
  );

  return $form;
}

/**
 * Plugin edit form submit handler.
 *
 * Saves the data from the form.
 */
function ding_item_list_plugin_edit_form_submit($form, &$form_state) {
  $form_state['conf']['item_query'] = $form_state['input']['item_query'];
  $form_state['conf']['item_count'] = $form_state['input']['item_count'];
}

/**
 * Plugin edit form validate handler.
 *
 * Validate the count for a numeric value.
 */
function ding_item_list_plugin_edit_form_validate($form, &$form_state) {
  $item_count = $form_state['input']['item_count'];

  if (!is_numeric($item_count)) {
    form_set_error('item_count', t('Only numeric value is allowed.'));
  }
}
