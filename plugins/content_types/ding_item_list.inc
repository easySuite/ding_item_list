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

  $objects = ding_item_list_get_objects($item_query, $item_count);
  $markup = '';

  foreach ($objects as $object) {
    $markup .= theme('ding_item_list_item', array(
      'faust' => $object->id,
      'cover' => image_style_url('ding_item_list', $object->image),
      'title' => $object->title,
      'author' => $object->creator,
      'year' => $object->year,
      'rating' => $object->rating,
      'rating_count' => $object->rating_count,
      'review_count' => $object->comment_count,
      'loan_form' => render(ding_provider_get_form('ding_reservation_reserve_form', new DingReservationReservableEntity($object), TRUE)),
    ));
  }

  drupal_add_css(drupal_get_path('module', 'ding_item_list') . '/css/ding_item_list.css', 'file');
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

  // Cache the list for DING_ITEM_LIST_CACHE_TLL seconds.
  // Apply default cache settings only to new panes.
  if (empty($form_state['pane']->cache)) {
    $pane_cache = &$form_state['pane']->cache;
    $pane_cache['method'] = 'simple';
    $pane_cache['settings'] = array(
      'lifetime' => DING_ITEM_LIST_CACHE_TLL,
      'granularity' => 'none',
    );
    // Invalidate old cache.
    // @todo: check if any changes were introduced.
    $simple_cache_plugin = ctools_plugin_get_info('panels', 'cache');
    ctools_plugin_load_includes($simple_cache_plugin);
    $cid = panels_simple_cache_get_id($pane_cache['settings'], $form_state['display'],
      array(), $form_state['contexts'], $form_state['pane']);
    cache_clear_all($cid, 'cache');
  }
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
