<?php

/**
 * @file
 * Theme setting callbacks for kalatheme.
 */

/**
 * Implements hook_form_FORM_ID_alter().
 */
function kalatheme_form_system_theme_settings_alter(&$form, &$form_state) {
  // Don't add custom form elements to Kalatheme's settings page if Kalatheme
  // isn't the default theme.
  // Also, don't add custom form elements to a subtheme's settings page if it
  // isn't the default theme.
  global $theme_info;
  $theme_name_matches = array();
  preg_match('/^admin\/appearance\/settings\/([^\/]+)\/?$/', request_path(), $theme_name_matches);
  if (isset($theme_name_matches[1]) && $theme_info->name != $theme_name_matches[1]) {
    return;
  }

  // Need to pass this through to use list_allowed_values_string without errors.
  $field = array('type' => 'list_text');

  // Page title setting (only print on non-panel pages or always print).
  $form['page_title'] = array(
    '#type' => 'fieldset',
    '#title' => t('Page Title'),
    '#weight' => 41,
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#description' => t('By default, Kalatheme only displays page titles on pages that aren\'t rendered through Panels or Panelizer.
      If toggled on, this setting will cause Kalatheme to always print the page title, regardless of how the page is rendered.'),
  );
  $form['page_title']['always_show_page_title'] = array(
    '#type' => 'checkbox',
    '#title' => t('Always show page title.'),
    '#default_value' => theme_get_setting('always_show_page_title'),
    '#description' => t('Check here to always print page titles.'),
  );

  // Responsive style plugin settings.
  $form['responsive'] = array(
    '#type' => 'fieldset',
    '#title' => t('Responsive'),
    '#weight' => 42,
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#description' => t('If toggled on, the kalacustomize style plugin will allow the user to configure device visibilty for regions and panes.'),
  );
  $form['responsive']['responsive_toggle'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use responsive toggling.'),
    '#default_value' => theme_get_setting('responsive_toggle'),
    '#description' => t('Check here if you want the user to be able to set the responsive visbility of each panels pane and region.'),
  );

  // Panels styles style plugin settings.
  $form['pane_styles'] = array(
    '#type' => 'fieldset',
    '#title' => t('Pane Styles'),
    '#weight' => 43,
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#description' => t('If toggled on, the kalacustomize style plugin will allow the user to set a class for panels panes.'),
  );
  $form['pane_styles']['pane_styles_toggle'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use panels styles.'),
    '#default_value' => theme_get_setting('pane_styles_toggle'),
    '#description' => t('Check here if you want to set the class for each panels pane.'),
  );
  $form['pane_styles']['pane_styles_settings'] = array(
    '#type' => 'container',
    '#states' => array(
      'invisible' => array(
        'input[name="pane_styles_toggle"]' => array('checked' => FALSE),
      ),
    ),
  );
  // Set defaults here instead of info because it is an array.
  $pane_classes = (theme_get_setting('pane_classes')) ? list_allowed_values_string(theme_get_setting('pane_classes')) : '';
  $form['pane_styles']['pane_styles_settings']['pane_classes'] = array(
    '#type' => 'textarea',
    '#title' => t('Allowed values list'),
    '#default_value' => $pane_classes,
    '#rows' => 10,
    '#element_validate' => array('list_allowed_values_setting_validate'),
    '#field_has_data' => FALSE,
    '#field' => $field,
    '#field_type' => $field['type'],
    '#description' => '<p>' . t('The possible values this field can contain. Enter one value per line, in the format key|label.'),
  );

  // Extra styles style plugin settings.
  $form['extra_styles'] = array(
    '#type' => 'fieldset',
    '#title' => t('Extra Pane Styles'),
    '#weight' => 44,
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#description' => t('If toggled on, the kalacustomize style plugin will allow the user to set elements and classes for panels panes titles and content.'),
  );
  $form['extra_styles']['extra_styles_toggle'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use extra pane styles.'),
    '#default_value' => theme_get_setting('extra_styles_toggle'),
    '#description' => t('Check here if you want to customize the elements and classes for pane titles and content.'),
  );
  $form['extra_styles']['extra_styles_settings'] = array(
    '#type' => 'container',
    '#states' => array(
      'invisible' => array(
        'input[name="extra_styles_toggle"]' => array('checked' => FALSE),
      ),
    ),
  );
  // Set defaults here instead of info because it is an array.
  $extra_elements = (theme_get_setting('extra_elements')) ? list_allowed_values_string(theme_get_setting('extra_elements')) : '';
  $form['extra_styles']['extra_styles_settings']['extra_elements'] = array(
    '#type' => 'textarea',
    '#title' => t('Allowed elements'),
    '#default_value' => $extra_elements,
    '#rows' => 10,
    '#element_validate' => array('list_allowed_values_setting_validate'),
    '#field_has_data' => FALSE,
    '#field' => $field,
    '#field_type' => $field['type'],
    '#description' => '<p>' . t('The possible values this field can contain. Enter one value per line, in the format key|label.'),
  );
  // Set defaults here instead of info because it is an array.
  $extra_classes = (theme_get_setting('extra_classes')) ? list_allowed_values_string(theme_get_setting('extra_classes')) : '';
  $form['extra_styles']['extra_styles_settings']['extra_classes'] = array(
    '#type' => 'textarea',
    '#title' => t('Allowed classes'),
    '#default_value' => $extra_classes,
    '#rows' => 10,
    '#element_validate' => array('list_allowed_values_setting_validate'),
    '#field_has_data' => FALSE,
    '#field' => $field,
    '#field_type' => $field['type'],
    '#description' => '<p>' . t('The possible values this field can contain. Enter one value per line, in the format key|label.'),
  );

  // Region styles style plugin settings.
  $form['region_styles'] = array(
    '#type' => 'fieldset',
    '#title' => t('Region Styles'),
    '#weight' => 55,
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#description' => t('If toggled on, the kalacustomize style plugin will allow the user to set a class for panels regions.'),
  );
  $form['region_styles']['region_styles_toggle'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use region styles.'),
    '#default_value' => theme_get_setting('region_styles_toggle'),
    '#description' => t('Check here if you want to set the class for each panels region.'),
  );
  $form['region_styles']['region_styles_settings'] = array(
    '#type' => 'container',
    '#states' => array(
      'invisible' => array(
        'input[name="region_styles_toggle"]' => array('checked' => FALSE),
      ),
    ),
  );
  // Set defaults here instead of info because it is an array.
  $region_classes = (theme_get_setting('region_classes')) ? list_allowed_values_string(theme_get_setting('region_classes')) : '';
  $form['region_styles']['region_styles_settings']['region_classes'] = array(
    '#type' => 'textarea',
    '#title' => t('Allowed values list'),
    '#default_value' => $region_classes,
    '#rows' => 10,
    '#element_validate' => array('list_allowed_values_setting_validate'),
    '#field_has_data' => FALSE,
    '#field' => $field,
    '#field_type' => $field['type'],
    '#description' => '<p>' . t('The possible values this field can contain. Enter one value per line, in the format key|label.'),
  );
}
