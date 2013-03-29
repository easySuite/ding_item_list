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
  'edit form' => 'ding_related_items_plugin_edit_form',
);

/**
 * Plugin content renderer.
 */
function ding_related_items_plugin_render($subtype, $conf, $panel_args, $context) {
  $plugin_data = new stdClass();
  $plugin_data->content = '';
  $field_type = ($context->data->type == 'ding_page') ? 'field_ding_material' : 'field_materials';
  $field_content = field_view_field('node', $context->data, $field_type);
  $markup = '';
  $ting_objects = array();

  foreach ($field_content as $k => $v) {
    if (is_numeric($k)) {
      $objects = ding_item_list_get_objects($v['#object']->reply->localId, 1, TRUE);

      if (is_array($objects)) {
        foreach ($objects as $object) {
          $markup .= theme('ding_item_list_item', array(
            'faust' => $object->id,
            'cover' => (!empty($object->image) && file_exists($object->image)) ? image_style_url('ding_item_list', $object->image) : '/' . drupal_get_path('module', 'ding_item_list') . '/images/no-image-minil.png',
            'title' => $object->title,
            'author' => $object->creator,
            'year' => $object->year,
            'has_rating' => $object->has_rating,
            'rating' => $object->rating,
            'rating_count' => $object->rating_count,
            'review_count' => $object->comment_count,
            'loan_form' => drupal_render(module_invoke('ding_reservation', 'ding_entity_buttons', 'ding_entity', $object->primary_object, 'ajax')),
          ));
          $ting_objects[] = $object->primary_object;
        }
      }
    }
  }

  // Add ajax availability checks.
  module_invoke('ding_availability', 'ajax', $ting_objects, 'availability');

  // Empty by default, so if no content is to be rendered, ctools will skip
  // rendering the entire pane.
  $plugin_data->title = $plugin_data->content = '';
  if (!empty($markup)) {
    drupal_add_css(drupal_get_path('module', 'ding_item_list') . '/css/ding_item_list.css', 'file');
    $plugin_data->content = theme('ding_item_list_list', array('items' => $markup));
    $plugin_data->title = t(!empty($conf['item_list_custom_title']) ? $conf['item_list_custom_title'] : 'Related materials');
  }

  return $plugin_data;
}

/**
 * Plugin edit form.
 */
function ding_related_items_plugin_edit_form($form, &$form_state) {
  // Use specific title handling - hide pane title if no content is present.
  // Standard pane title handling does not provide such functionality,
  // so use custom title field and hide the standard ones.
  unset($form['override_title']);
  unset($form['override_title_text']);
  unset($form['override_title_markup']);

  $form['item_list_custom_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Pane title'),
    '#default_value' => t(!empty($form_state['conf']['item_list_custom_title']) ? $form_state['conf']['item_list_custom_title'] : 'Related materials'),
  );

  return $form;
}

/**
 * Plugin edit form submit handler.
 *
 * Saves the data from the form.
 */
function ding_related_items_plugin_edit_form_submit($form, &$form_state) {
  // Save custom pane title.
  $form_state['conf']['item_list_custom_title'] = $form_state['values']['item_list_custom_title'];

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
