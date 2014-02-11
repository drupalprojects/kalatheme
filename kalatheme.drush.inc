<?php
/**
 * @file
 * Drushhhhhhhh
 */

// Constants and stuff
define('DEFAULT_BOOTSTRAP_URL', 'https://github.com/twbs/bootstrap/releases/download/v3.1.0/bootstrap-3.1.0-dist.zip');
define('DEFAULT_BOOTSWATCH_URL', 'http://bootswatch.com');
define('DEFAULT_FONTAWESOME_URL', 'http://fontawesome.io/./assets/font-awesome-4.0.3.zip');

/**
 * Implements hook_drush_command().
 */
function kalatheme_drush_command() {
  $items = array();

  $items['kalatheme'] = array(
    'description' => 'Create a Kalatheme subtheme.',
    'arguments' => array(
      'name'         => 'The name of your subtheme.',
    ),
    'options' => array(
      'destination' => 'The destination of your subtheme. Defaults to sites/all/themes.',
      'base-theme' => 'This will let you use a different basetheme than kalatheme.'
    ),
    'examples' => array(
      'drush kalatheme "My Awesome Theme"' => 'Creates a kalatheme subtheme called "My Theme"',
    ),
  );

  return $items;
}

/**
 * Implements hook_drush_help().
 */
function kalatheme_drush_help($section) {
  switch ($section) {
    case 'drush:kalatheme':
      return dt('This command will create a Kalatheme subtheme.');
  }
}

/**
 * Implements drush_hook_command().
 */
function drush_kalatheme($name = NULL) {
  // Get arguments
  if (!$name) {
    drush_print(dt('You need a theme name. See help using drush kalatheme --help.'));
    return;
  }
  if (!isset($name)) {
    $name = drush_get_option('name');
  }
  $machine_name = strtolower(addslashes($name));
  $machine_name = preg_replace('@[^a-z0-9_]+@', '_', $machine_name);

  // Get options
  $subtheme_path = 'sites/all/themes';
  if ($destination = drush_get_option('destination')) {
    $subtheme_path = drush_trim_path($destination);
  }
  $subtheme_path = drush_normalize_path(drush_get_context('DRUSH_DRUPAL_ROOT') . '/' . $subtheme_path);
  $library_path = drush_normalize_path(drush_get_context('DRUSH_DRUPAL_ROOT') . '/sites/all/libraries/' . $machine_name . '_bootstrap');
  $custom = FALSE;
  if (drush_get_option('custom')) {
    $custom = drush_get_option('custom');
  }
  $base_theme = 'kalatheme';
  if (drush_get_option('base-theme')) {
    $base_theme = drush_get_option('base-theme');
  }

  $current_dir = getcwd();
  chdir(drush_tempdir());
  // Get bootstrap library
  if ($filepath = drush_download_file(DEFAULT_BOOTSTRAP_URL)) {
    $filename = basename($filepath);
    $dirname =  basename($filepath, '.zip');

    // Decompress the zip archive.
    drush_tarball_extract($filename, $dirname, TRUE);
    drush_move_dir($dirname . '/dist', $library_path, TRUE);
  }

  // Build subtheme
  $subtheme_files = drush_op('_kalatheme_drush_build_subtheme', $name, $machine_name, $base_theme);
  foreach ($subtheme_files as $key => $value) {
    $dir = substr($key, 0, strrpos($key, '/'));
    if (is_dir($dir) || mkdir($dir, 0777, TRUE)) {
      if (!file_put_contents($key, $value)) {
        return FALSE;
      }
    }
    else {
      return FALSE;
    }
  }
  drush_move_dir($machine_name, $subtheme_path . DIRECTORY_SEPARATOR . $machine_name, TRUE);

  chdir($current_dir);

  // Notify user of the newly created theme.
  $message = dt('kalatheme subtheme "!name" created in: !path using the "!kit" kit.', array(
    '!name' => $name,
    '!path' => $subtheme_path,
    '!kit' => $kit,
  ));
  drush_print($message);
}

/**
 * Construct an array of subtheme things.
 *
 * @param $params
 *   The subtheme parameters including:
 *    - name: the subtheme name
 *    - base: the subtheme base theme
 * @param $context
 *   Batch things
 *
 */
function _kalatheme_drush_build_subtheme($name, $machine_name, $base_theme = 'kalatheme') {
  $subtheme = array();
  // Basic Info File
  $output  = "name = " . $name . " \n";
  $output .= "description = A Kalatheme subtheme powered by Twitter Bootstrap & Science. \n";
  $output .= "base theme = " . $base_theme . " \n";
  $output .= "core = 7.x \n";
  $output .= "\n";
  $output .= "regions[content] = Content \n";
  $output .= "\n";
  $output .= "settings[toggle_logo] = 0 \n";
  $output .= "\n";
  $output .= "stylesheets[all][] = css/main.css \n";
  $output .= "\n";
  $subtheme[$machine_name . DIRECTORY_SEPARATOR . $machine_name . '.info'] = $output;
  // Basic CSS File
  $output  = "/*! \n";
  $output .= " * Kalatheme (http://drupal.org/project/kalatheme) \n";
  $output .= " */ \n";
  $subtheme[$machine_name . DIRECTORY_SEPARATOR . 'css' . DIRECTORY_SEPARATOR . 'main.css'] = $output;
  // Basic Template File
  $output  = "<?php \n";
  $output .= "/** \n";
  $output .= " * @file \n";
  $output .= " * Primary pre/preprocess functions and alterations.\n";
  $output .= " */ \n";
  $subtheme[$machine_name . DIRECTORY_SEPARATOR . 'template.php'] = $output;

  return $subtheme;
}
