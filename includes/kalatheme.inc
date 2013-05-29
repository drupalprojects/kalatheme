<?php

/**
 * @file
 * Kalatheme's helper functions
 */

/**
 * Message displayed when user does not have Panopoly installed
 */
define('KALATHEME_PANOPOLY_ERROR', t('In order to use Kalatheme,
  you must have the Panopoly distribution installed.
  Please check the README for installation instructions.'));
/**
 * Message displayed when user does not have Bootstrap available
 */
define('KALATHEME_BOOTSTRAP_ERROR', t('In order to use Kalatheme, you must
  have the Twitter Bootstrap library available. Please check the README for
  installation instructions.'));

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
  // Add what we need from Bootstrap and make sure Panopoly is installed.
  $all_loaded = FALSE;
  if (module_exists('panopoly_core')) {
    $library = libraries_detect('bootstrap');
    if ($library && !empty($library['installed'])) {
      $bootstrap_path = DRUPAL_ROOT . '/' . $library['library path'];
      $variant = NULL;
      $has_minified_css = file_exists($bootstrap_path . '/css/bootstrap.min.css');
      $has_minified_js = file_exists($bootstrap_path . '/js/bootstrap.min.js');
      if ($has_minified_css && $has_minified_js) {
        $variant = 'minified';
      }
      libraries_load('bootstrap', $variant);
      $all_loaded = TRUE;
    }
    else {
      // Something went wrong. Sadface.
      drupal_set_message(KALATHEME_BOOTSTRAP_ERROR, 'error');
    }
  }
  else {
    drupal_set_message(KALATHEME_PANOPOLY_ERROR, 'error');
  }
  // Return the correct status message function depending on whether
  // we were able to load all of our dependencies.
  if (!$all_loaded) {
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
    'error' => 'error',
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
 * This attempts to find and return the Bootstrap library version
 * 
 * @param $library - The actual library
 * @param $options - Options to help determine the library version
 * @return Library version number
 */
function _kalatheme_get_version($library, $options) {
  // Use bootstrap.min.css if exists, if not use normal bootstrap.css
  $file = (file_exists(DRUPAL_ROOT . '/' . $library['library path'] . '/css/bootstrap.min.css')) ? 
    '/css/bootstrap.min.css' : '/css/bootstrap.css';
  
  // Provide defaults.
  $options += array(
    'file' => $file,
    'pattern' => '',
    'lines' => 20,
    'cols' => 200,
  );

  $file = DRUPAL_ROOT . '/' . $library['library path'] . '/' . $options['file'];
  if (empty($options['file']) || !file_exists($file)) {
    return;
  }
  $file = fopen($file, 'r');
  while ($options['lines'] && $line = fgets($file, $options['cols'])) {
    if (preg_match($options['pattern'], $line, $version)) {
      fclose($file);
      return $version[1];
    }
    $options['lines']--;
  }
  fclose($file);
}
