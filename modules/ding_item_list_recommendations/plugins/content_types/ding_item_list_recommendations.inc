<?php
/**
 * @file
 * Recommendation list pane plugin.
 */

$plugin = array(
  'title' => t('Ding item list recommendations'),
  'category' => t('Ding!'),
  'description' => t('Show recomendation list as a ding item list'),
  'single' => TRUE,
  'render callback' => 'ding_item_list_recommendations_plugin_render',
  'edit form' => 'ding_item_list_recommendations_plugin_edit_form',
  'required context' => array(
    new ctools_context_optional(t('Ting object'), 'ting_object'),
    new ctools_context_optional(t('Ting collection'), 'ting_collection'),
  ),
);

/**
 * Plugin renderer.
 */
function ding_item_list_recommendations_plugin_render($subtype, $conf, $panel_args, $context) {
  $plugin_data = new stdClass();
  $items_show = isset($conf['recommendations_list_item_list_show']) ? $conf['recommendations_list_item_list_show'] : 10;

  if (isset($context[0])) {
    $objects = NULL;

    if ($context[0]->type == 'ting_object') {
      $objects = ting_get_object($context[0]->get_original_argument()->getId());
    }
    elseif ($context[0]->type == 'ting_collection') {
      $objects = ting_get_object($context[0]->get_original_argument()->getId(), TRUE);
    }

    if ($objects) {
      $identifiers = ding_adhl_frontend_extract_identifers($objects);
      $local_ids = array();

      foreach ($identifiers as $identifier) {
        $recommendations = ting_get_object_recommendations($identifier, $items_show * 2);

        if (isset($recommendations) && is_array($recommendations)) {
          foreach ($recommendations as $recommendation) {
            $local_ids[] = $recommendation->localId;
          }
        }
      }

      $query = implode(' OR ', $local_ids);
      $markup = ding_item_list_get_content($query, $items_show, '');

      $plugin_data->content = $markup;
    }
  }

  $plugin_data->title = t(isset($conf['recommendations_list_custom_title']) ? $conf['recommendations_list_custom_title'] : 'Others have borrowed');

  return $plugin_data;
}

/**
 * Plugin configuration form.
 */
function ding_item_list_recommendations_plugin_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];

  // Use specific title handling - hide pane title if no content is present.
  // Standard pane title handling does not provide such functionality,
  // so use custom title field and hide the standard ones.
  unset($form['override_title']);
  unset($form['override_title_text']);
  unset($form['override_title_markup']);

  $form['recommendations_list_custom_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Pane title'),
    '#default_value' => t(isset($conf['recommendations_list_custom_title']) ? $conf['recommendations_list_custom_title'] : 'Others have borrowed'),
  );

  $form['recommendations_list_item_list_show'] = array(
    '#type' => 'textfield',
    '#title' => t('Items to show'),
    '#default_value' => isset($conf['recommendations_list_item_list_show']) ? $conf['recommendations_list_item_list_show'] : 10,
    '#element_validate' => array('element_validate_integer_positive'),
    '#description' => t('Actual results may vary.'),
  );

  return $form;
}

/**
 * Plugin edit form submit handler.
 *
 * @see ding_item_list_recommendations_plugin_edit_form()
 */
function ding_item_list_recommendations_plugin_edit_form_submit($form, &$form_state) {
  $form_state['conf']['recommendations_list_custom_title'] = $form_state['input']['recommendations_list_custom_title'];
  $form_state['conf']['recommendations_list_item_list_show'] = $form_state['input']['recommendations_list_item_list_show'];
}
