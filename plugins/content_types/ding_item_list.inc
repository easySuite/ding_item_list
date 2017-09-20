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
  'render callback' => 'ding_item_list_plugin_render',
  'edit form' => 'ding_item_list_plugin_edit_form',
);

/**
 * Plugin content renderer.
 */
function ding_item_list_plugin_render($subtype, $conf, $panel_args, $context) {
  // Add cron job callback to be able to warmup the cache.
  if (!empty($conf['item_query'])) {
    _ding_item_list_cronapi_task($conf['item_query'], __FUNCTION__, func_get_args());
  }

  $plugin_data = new stdClass();
  $plugin_data->content = '';

  $item_sort = empty($conf['item_sort']) ? '' : $conf['item_sort'];

  $params = implode('', array(
    'query' => $conf['item_query'],
    'count' => $conf['item_count'],
    'sort' => $item_sort,
  ));
  $hash = md5($params);

  $path = drupal_get_path('module', 'ding_item_list');
  $plugin_data->content['#markup'] = theme('ding_item_list_list', array('hash' => $hash));
  $plugin_data->content['#attached'] = array(
    'js' => array(
      array('data' => $path . '/js/ding_item_list.js'),
      array('data' => $path . '/js/ding-list-add-button.js'),
      array(
        'data' => array(
          $hash => array(
            'query' => $conf['item_query'],
            'count' => $conf['item_count'],
            'sort' => $item_sort,
          ),
        ),
        'type' => 'setting'
      ),
    ),
    'css' => array(
      $path . '/css/ding_item_list.css',
      $path . '/css/ding-list-add-button.css',
    ),
  );

  $plugin_data->title = t(isset($conf['item_list_item_list_custom_title']) ? $conf['item_list_item_list_custom_title'] : 'Related materials');

  return $plugin_data;
}
