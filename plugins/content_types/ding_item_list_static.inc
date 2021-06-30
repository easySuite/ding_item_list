<?php
/**
 * @file
 * Define pane with static loaded data.
 */

/**
 * General ctools plugin settings.
 */
$plugin = array(
  'title' => t('Ding item list Static'),
  'category' => t('Ding!'),
  'description' => t('Add ting items as a list'),
  'single' => TRUE,
  'render callback' => 'ding_item_list_static_plugin_render',
  'edit form' => 'ding_item_list_plugin_edit_form',
);

/**
 * Render callback of the pane.
 *
 * @return \stdClass
 */
function ding_item_list_static_plugin_render($subtype, $conf, $panel_args, $context) {
  // Add cron job callback to be able to warmup the cache.
  if (!empty($conf['item_query'])) {
    _ding_item_list_cronapi_task($conf['item_query'], __FUNCTION__, func_get_args());
  }

  $plugin_data = new stdClass();
  $plugin_data->title = t(isset($conf['item_list_item_list_custom_title']) ? $conf['item_list_item_list_custom_title'] : 'Materials');

  $item_sort = empty($conf['item_sort']) ? '' : $conf['item_sort'];

  $markup = ding_item_list_get_static_content($conf['item_query'], (int) $conf['item_count'], $item_sort, $conf['no_covers']);
  $plugin_data->content = $markup;

  return $plugin_data;
}

/**
 * Generate markup according to passed search parameters.
 *
 * @param string $query
 *   Search query.
 * @param int $count
 *   Number of items to fetch.
 * @param string $sort
 *   Sort identifier.
 * @param string $no_covers
 *   Fetch items with covers or not.
 *
 * @return string
 *   List markup.
 */
function ding_item_list_get_static_content($query, $count, $sort, $no_covers = NULL) {
  $cid = ding_item_list_generate_cache_id($query . $count . $sort);
  $cache = cache_get($cid);

  if (!$cache || empty($cache->data)) {
    $objects = _ding_item_list_get_items_with_covers($query, $count, $sort, $no_covers);
    cache_set($cid, $objects, 'cache', $_SERVER['REQUEST_TIME'] + DING_ITEM_LIST_CACHE_TLL);
  }
  else {
    $objects = $cache->data;
  }

  $markup = '';
  foreach ($objects as $object) {
    $cover = ting_covers_object_path($object->getId());
    $creators = $object->getCreators();

    $theme_variables = [
      'faust' => $object->getId(),
      'cover' => (!empty($cover) && file_exists($cover)) ? image_style_url('ding_item_list', $cover) : '/' . drupal_get_path('module', 'ding_item_list') . '/images/no-image-minil.png',
      'title' => $object->getTitle(),
      'author' => reset($creators),
      'year' => $object->getDate(),
      'description' => mb_strimwidth($object->getAbstract(), 0, 50, '...'),
    ];

    if (module_exists('ding_entity_rating')) {
      $theme_variables['rating'] = ding_entity_rating_get_average($object);
    }

    $markup .= theme('ding_item_list_item_table', $theme_variables);
  }

  return $markup;
}
