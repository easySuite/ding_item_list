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
  $plugin_data->title = t(isset($conf['item_list_item_list_custom_title']) ? $conf['item_list_item_list_custom_title'] : 'Related materials');

  $item_sort = empty($conf['item_sort']) ? '' : $conf['item_sort'];

  $markup = ding_item_list_get_static_content($conf['item_query'], $conf['item_count'], $item_sort);
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
 *   Sort identifier
 *
 * @return string
 *   List markup.
 */
function ding_item_list_get_static_content($query, $count, $sort) {
  $cid = ding_item_list_generate_cache_id($query . $count . $sort);
  $cache = cache_get($cid);

  if (!$cache || empty($cache->data)) {
    $objects = ding_item_list_get_objects($query, $count, FALSE, $sort);
    cache_set($cid, $objects, 'cache', $_SERVER['REQUEST_TIME'] + DING_ITEM_LIST_CACHE_TLL);
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
        'cover' => (!empty($object->image) && file_exists($object->image)) ? image_style_url('ding_item_list', $object->image) : '/' . drupal_get_path('module', 'ding_item_list') . '/images/no-image-minil.png',
        'title' => $object->title,
        'author' => $object->creator,
        'year' => $object->year,
        'has_rating' => $object->has_rating,
        'rating' => $object->rating,
        'rating_count' => $object->rating_count,
        'review_count' => $object->comment_count,
        'description' => mb_strimwidth($object->description, 0, 50, '...'),
      )
    );
  }

  return $markup;
}
