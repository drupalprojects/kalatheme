<?php

/**
 * @file
 * Kalatheme's helper functions
 */

/**
 * Loads all dependencies necessary for Kalatheme to operate.
 *
 * @return string
 *   Name of the function for kalatheme_status_messages to call
 */
function _kalatheme_load_dependencies() {
  // If we've already loaded dependencies, just return the correct function to
  // call for status messages.
  $status_function = &drupal_static(__FUNCTION__);
  if (isset($status_function)) {
    return $status_function;
  }

  // Check some minimal module, variable and bootstrap reqs
  // See if we are running a 3.x version of panels
  $panels = FALSE;
  if (module_exists('panels')) {
    $info = system_get_info('module', 'panels');
    $regex = '/^7\.x-3\.[0-9]+.*$/';
    // Check major version
    if (preg_match($regex, $info['version'])) {
      $minor = substr($info['version'], 6);
      if (strpos($info['version'], 'dev') !== FALSE) {
        // Keep commit add for potential future use
        $dev = substr(substr($minor, strpos($minor, '+')+1), 0, -4);
        $minor = substr($minor, 0, strpos($minor, '+'));
      }
      if ($minor >= 3) {
        $panels = TRUE;
      }
    }
    elseif (!$info['version']) {
      $panels = TRUE;
    }
  }
  if (!$panels) {
    drupal_set_message(t('Kalatheme requires at least Panels 3.3.'), 'error');
  }
  // See if we are running at least 2.1 of libraries
  $libs = FALSE;
  if (module_exists('libraries')) {
    $info = system_get_info('module', 'libraries');
    $regex = '/^7\.x-2\.[1-9][0-9]*.*$/';
    if (preg_match($regex, $info['version']) || !$info['version']) {
      $libs = TRUE;
    }

  }
  if (!$libs) {
    drupal_set_message(t('Kalatheme requires at least Libraries 2.1.'), 'error');
  }
  // Check to see if bootstrap libraries exist
  $boot = FALSE;
  if (module_exists('libraries')) {
    $bootstrap = libraries_detect(KALATHEME_BOOTSTRAP_LIBRARY);
    if ($bootstrap && !empty($bootstrap['installed'])) {
      $bootstrap_path = DRUPAL_ROOT . '/' . $bootstrap['library path'];
      $variant = NULL;
      $has_minified_css = file_exists($bootstrap_path . '/css/bootstrap.min.css');
      $has_minified_js = file_exists($bootstrap_path . '/js/bootstrap.min.js');
      if ($has_minified_css && $has_minified_js) {
        $variant = 'minified';
      }
      libraries_load(KALATHEME_BOOTSTRAP_LIBRARY, $variant);
      $boot = TRUE;
    }
  }
  if (!$boot) {
    drupal_set_message(t('You do not have a Bootstrap library installed but that is ok! To get equipped either check out our <a href="@setup_guide">Start Up Guide</a> or run our <a href="@setup_url">Setup Wizard.</a>', array(
      '@setup_guide' => 'https://drupal.org/node/2167149',
      '@setup_url' => url('admin/appearance/kalasetup'),
    )), 'error');
  }
  // See if we are running at least a 2.x branch of jquery_update
  // Check to see if we are using jquery 1.7+
  $jq = FALSE;
  if (module_exists('jquery_update')) {
    $info = system_get_info('module', 'jquery_update');
    $regex = '/^7\.x-2\.[0-9]+.*$/';
    $version = explode('.', variable_get('jquery_update_jquery_version'));
    if ($version[0] ==1 && $version >= 7) {
      $jq = TRUE;
    }
  }
  if (!$jq) {
    drupal_set_message(t('Kalatheme requires a 2.x version of JQuery Update that uses at
      least JQuery 1.7'), 'error');
  }
  // Return the correct status message function depending on whether
  // we were able to load all of our Bootstrap dependencies.
  if (!$boot || !$jq) {
    $status_function = 'theme_status_messages';
  }
  else {
    $status_function = '_kalatheme_status_messages';
  }
  return $status_function;
}

/**
 * Helper function to filter element children.
 *
 * See http://api.drupal.org/comment/34713#comment-34713
 */
function _kalatheme_element_children($element) {
  return array_intersect_key(
    $element,
    array_flip(element_children($element))
  );
}

/**
 * Associate all the secondary menu tasks as #children of primary tasks.
 */
function _kalatheme_associate_parent_tasks($variables) {
  // Assign all secondary links as children of their parent primary link.
  foreach (_kalatheme_element_children($variables['secondary']) as $secondary_index => $secondary_link) {
    $link = $secondary_link['#link'];
    foreach (_kalatheme_element_children($variables['primary']) as $primary_index => $primary_link) {
      if (!isset($primary_link['#markup']) && $primary_link['#link']['path'] == $link['tab_parent']) {
        $variables['primary'][$primary_index]['#children'][] = $secondary_link;
        unset($variables['secondary'][$secondary_index]);
      }
    }
  }

  // If a secondary link hasn't been assigned, make it a primary link.
  // @todo: this should never happen; consider removing?
  foreach ($variables['secondary'] as $secondary_link) {
    $variables['primary'][] = $secondary_link;
  }

  return $variables;
}

/**
 * Returns HTML for status and/or error messages, grouped by type.
 */
function _kalatheme_status_messages($variables) {
  $display = $variables['display'];
  $output = '';

  $status_heading = array(
    'status' => t('Status message'),
    'error' => t('Error message'),
    'warning' => t('Warning message'),
  );

  // Map Drupal message types to their corresponding Bootstrap classes.
  // @see http://twitter.github.com/bootstrap/components.html#alerts
  $status_class = array(
    'status' => 'success',
    'error' => 'danger',
    'warning' => 'info',
  );

  foreach (drupal_get_messages($display) as $type => $messages) {
    $class = (isset($status_class[$type])) ? ' alert-' . $status_class[$type] : '';
    $output .= "<div class=\"alert alert-block$class\">\n";
    $output .= "  <a class=\"close\" data-dismiss=\"alert\" href=\"#\">x</a>\n";

    if (!empty($status_heading[$type])) {
      $output .= '<h2 class="element-invisible">' . $status_heading[$type] . "</h2>\n";
    }

    if (count($messages) > 1) {
      $output .= " <ul>\n";
      foreach ($messages as $message) {
        $output .= '  <li>' . $message . "</li>\n";
      }
      $output .= " </ul>\n";
    }
    else {
      $output .= $messages[0];
    }

    $output .= "</div>\n";
  }
  return $output;
}

/**
 * Returns a list of Kalatheme base themes.
 */
function _kalatheme_get_base_themes() {
  $themes = list_themes();

  $base_themes = array('kalatheme' => 'Kalatheme');
  if (isset($themes['kalatheme']->sub_themes)) {
    $base_themes += $themes['kalatheme']->sub_themes;
  }

  return $base_themes;
}
