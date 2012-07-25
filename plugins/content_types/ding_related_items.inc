<?php
/**
 * @file
 * CTools plugin definition file.
 */

/**
 * General ctools plugin settings.
 */
$plugin = array(
  'title' => t('Ding related items'),
  'category' => t('Ding!'),
  'description' => t('Display related ting items for a certain node'),
  'single' => TRUE,
  'required context' => new ctools_context_required(t('Node'), 'node'),
  'edit form' => 'ding_related_items_content_type_edit_form',
  'render callback' => 'ding_related_items_plugin_render',
  'edit form' => 'ding_related_items_plugin_edit_form'
);

/**
 * Plugin content renderer.
 */
function ding_related_items_plugin_render($subtype, $conf, $panel_args, $context) {
  $plugin_data = new stdClass();
  $plugin_data->content = '';
  $field_content = field_view_field('node', $context->data, 'field_materials');

  foreach ($field_content as $k => $v) {
    if (is_numeric($k)) {
      $objects = ding_item_list_get_objects($v['#object']->reply->localId, 1, TRUE);

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
          'loan_form' => render(ding_provider_get_form('ding_reservation_reserve_form', new DingReservationReservableEntity($object), TRUE))
        ));
      }
    }
  }

  drupal_add_css(drupal_get_path('module', 'ding_item_list') . '/css/ding_item_list.css', 'file');
  $plugin_data->content = theme('ding_item_list_list', array('items' => $markup));

  return $plugin_data;
}

/**
 * Plugin edit form.
 */
function ding_related_items_plugin_edit_form($form, &$form_state) {

  return $form;
}

/**
 * Plugin edit form submit handler.
 *
 * Saves the data from the form.
 */
function ding_related_items_plugin_edit_form_submit($form, &$form_state) {
  // Cache the list for DING_ITEM_LIST_CACHE_TLL seconds.
  // Apply default cache settings only to new panes.
  if (empty($form_state['pane']->cache)) {
    $pane_cache = &$form_state['pane']->cache;
    $pane_cache['method'] = 'simple';
    $pane_cache['settings'] = array(
      'lifetime' => DING_ITEM_LIST_CACHE_TLL,
      'granularity' => 'none');
    // Invalidate old cache.
    // @todo: check if any changes were introduced.
    $simple_cache_plugin = ctools_plugin_get_info('panels', 'cache');
    ctools_plugin_load_includes($simple_cache_plugin);
    $cid = panels_simple_cache_get_id($pane_cache['settings'], $form_state['display'],
      array(), $form_state['contexts'], $form_state['pane']);
    cache_clear_all($cid, 'cache');
  }
}
