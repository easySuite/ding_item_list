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
  'edit form' => 'ding_item_list_content_type_edit_form',
  'render callback' => 'ding_item_list_plugin_render',
  'edit form' => 'ding_item_list_plugin_edit_form',
);

/**
 * Plugin content renderer.
 */
function ding_item_list_plugin_render($subtype, $conf, $panel_args, $context) {
  $plugin_data = new stdClass();
  $plugin_data->content = '';

  $item_query = $conf['item_query'];
  $item_count = $conf['item_count'];

  $cid = ding_item_list_generate_cache_id($item_query);
  $cache = cache_get($cid, 'cache_ding_item');
  if (!$cache) {
    $objects = ding_item_list_get_objects($item_query, $item_count);
    cache_set($cid, $objects, 'cache_ding_item', time() + DING_ITEM_LIST_CACHE_TLL * 3600);
  }
  else {
    $objects = $cache->data;
  }

  // Check if cover images are physically present.
  try {
    ding_item_list_create_missed_covers($objects);
  }
  catch (Exception $e) {
    // No special handling at the moment.
  }

  $markup = '';
  foreach ($objects as $object) {
    $markup .= theme('ding_item_list_item', array(
      'faust' => $object->id,
      'cover' => image_style_url('ding_item_list', $object->image),
      'title' => $object->title,
      'author' => $object->creator,
      'year' => $object->year,
      'has_rating' => $object->has_rating,
      'rating' => $object->rating,
      'rating_count' => $object->rating_count,
      'review_count' => $object->comment_count,
      'loan_form' => drupal_render(module_invoke('ding_reservation', 'ding_entity_buttons', 'ding_entity', $object->primary_object, 'ajax')),
    ));
  }

  drupal_add_css(drupal_get_path('module', 'ding_item_list') . '/css/ding_item_list.css', 'file');
  $plugin_data->content = theme('ding_item_list_list', array('items' => $markup));
  $plugin_data->title = isset($conf['item_list_item_list_custom_title']) ? t($conf['item_list_item_list_custom_title']) : t('Related materials');

  return $plugin_data;
}

/**
 * Plugin edit form.
 */
function ding_item_list_plugin_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];

  // Use specific title handling - hide pane title if no content is present.
  // Standard pane title handling does not provide such functionality,
  // so use custom title field and hide the standard ones.
  unset($form['override_title']);
  unset($form['override_title_text']);
  unset($form['override_title_markup']);

  $form['item_list_item_list_custom_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Pane title'),
    '#default_value' => isset($conf['item_list_item_list_custom_title']) ? $conf['item_list_item_list_custom_title'] : t('Related materials'),
  );

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
 * Saves the data from the form.
 */
function ding_item_list_plugin_edit_form_submit($form, &$form_state) {
  $form_state['conf']['item_list_item_list_custom_title'] = $form_state['values']['item_list_item_list_custom_title'];
  $form_state['conf']['item_query'] = $form_state['input']['item_query'];
  $form_state['conf']['item_count'] = $form_state['input']['item_count'];
}

/**
 * Plugin edit form validate handler.
 * Validate the count for a numeric value.
 */
function ding_item_list_plugin_edit_form_validate($form, &$form_state) {
  $item_count = $form_state['input']['item_count'];

  if (!is_numeric($item_count)) {
    form_set_error('item_count', t('Only numeric value is allowed.'));
  }
}
