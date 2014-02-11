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
      'base-theme' => 'This will let you use a different basetheme than kalatheme.',
      'custom' => 'The name of a Bootstwatch theme or a URL containing a custom Bootstrap library archive.',
      'fonts' => 'Add FontAwesome 4 support.',
    ),
    'examples' => array(
      'drush kalatheme "My Awesome Theme"' => 'Creates a kalatheme subtheme called "My Theme"',
      'drush kalatheme "My Awesome Theme" --fonts' => 'Creates a kalatheme subtheme called "My Theme" with Font Awesome installed and configured.',
      'drush kalatheme "My Awesome Theme" --custom=yeti' => 'Creates a kalatheme subtheme called "My Theme" with the Yeti Bootstwatch theme.',
      'drush kalatheme "My Awesome Theme" --base-theme=kalasubtheme' => 'Creates a kalatheme subtheme called "My Theme" but sets its basetheme to "kalsubtheme"',
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
  // Get arguments and parse the machine name
  if (!$name) {
    drush_print(dt('You need a theme name.'));
    return;
  }
  if (!isset($name)) {
    $name = drush_get_option('name');
  }
  $machine_name = strtolower(addslashes($name));
  $machine_name = preg_replace('@[^a-z0-9_]+@', '_', $machine_name);
  // Get options
  // Get paths to theme and libraries folders
  $subtheme_path = 'sites/all/themes';
  if ($destination = drush_get_option('destination')) {
    $subtheme_path = drush_trim_path($destination);
  }
  $subtheme_path = drush_normalize_path(drush_get_context('DRUSH_DRUPAL_ROOT') . '/' . $subtheme_path);
  $library_path = drush_normalize_path(drush_get_context('DRUSH_DRUPAL_ROOT') . '/sites/all/libraries/' . $machine_name . '_bootstrap');
  // See if we are going to use a custom bootswatch theme or pull in a custom
  // bootstrap library from a URL archive
  $custom = FALSE;
  if (drush_get_option('custom')) {
    $bootswatch = array('amelia', 'cerulean', 'cosmo', 'cyborg', 'flatly', 'journal', 'readable', 'simplex', 'slate', 'spacelab', 'united', 'yeti');
    $custom = drush_get_option('custom');
    if (!filter_var($custom, FILTER_VALIDATE_URL) && !in_array($custom, $bootswatch)) {
      drush_print(dt('Not a valid URL or Bootstwatch theme.'));
      return;
    }
  }
  // Use a custom subtehme basetheme instead of kalatheme
  // @todo: we might want to check to see if the users input is actually
  // a kalatheme basetheme
  $base_theme = 'kalatheme';
  if (drush_get_option('base-theme')) {
    $base_theme = drush_get_option('base-theme');
  }
  // Get FONT AWESOME
  $fonts = FALSE;
  if (drush_get_option('fonts')) {
    $fonts = drush_get_option('fonts');
  }

  // Begin the science
  // Download all the stuff we need
  $current_dir = getcwd();
  chdir(drush_tempdir());
  // Get default bootstrap library
  // @todo check to see if we have a custom URL first
  if ($filepath = drush_download_file(DEFAULT_BOOTSTRAP_URL)) {
    $filename = basename($filepath);
    $dirname =  basename($filepath, '.zip');
    // Extract the archive.
    drush_tarball_extract($filename, $dirname, TRUE);
    // Pull in bootstwatch if we have a custom theme
    if ($custom && !filter_var($custom, FILTER_VALIDATE_URL)) {
      $files = array('bootstrap.css', 'bootstrap.min.css');
      foreach ($files as $file) {
        drush_download_file(DEFAULT_BOOTSWATCH_URL . '/' . $custom . '/' . $file, $dirname . '/dist/css/' . $file);
      }
    }
    // Move out of temp and into your life
    // copy and overwrite so we can toggle themes
    drush_copy_dir($dirname . '/dist', $library_path, FILE_EXISTS_OVERWRITE);
  }
  // Build basic subtheme files
  $subtheme_files = drush_op('_kalatheme_drush_build_subtheme', $name, $machine_name, $base_theme);
  // Add font awesome CSS
  if ($fonts) {
    $output  = "stylesheets[all][] = css/font-awesome.min.css \n";
    $output .= "\n";
    $subtheme_files[$machine_name . '/' . $machine_name . '.info'] .= $output;
  }
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
  // Use copy because we want to merge
  drush_move_dir($machine_name, $subtheme_path . '/' . $machine_name, TRUE);

  // Add Font Awesome assets
  if ($fonts) {
    if ($filepath = drush_download_file(DEFAULT_FONTAWESOME_URL)) {
      $filename = basename($filepath);
      $dirname =  basename($filepath, '.zip');
      // Extract the archive.
      drush_tarball_extract($filename, $dirname, TRUE);
      // Move out of temp and into your life
      $fonts_dir = $dirname . '/' . $dirname . '/fonts';
      $css_dir = $dirname . '/' . $dirname . '/css';
      drush_copy_dir($fonts_dir, $subtheme_path . '/' . $machine_name . '/fonts', FILE_EXISTS_MERGE);
      drush_copy_dir($css_dir, $subtheme_path . '/' . $machine_name . '/css', FILE_EXISTS_MERGE);
    }
  }

  // Switch back to current dir
  chdir($current_dir);
  // Notify user of the new polution
  $message = dt('kalatheme subtheme "!name" created in: !path.', array(
    '!name' => $name,
    '!path' => $subtheme_path,
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
  $subtheme[$machine_name . '/' . $machine_name . '.info'] = $output;
  // Basic CSS File
  $output  = "/*! \n";
  $output .= " * Kalatheme (http://drupal.org/project/kalatheme) \n";
  $output .= " */ \n";
  $subtheme[$machine_name . '/' . 'css' . '/' . 'main.css'] = $output;
  // Basic Template File
  $output  = "<?php \n";
  $output .= "/** \n";
  $output .= " * @file \n";
  $output .= " * Primary pre/preprocess functions and alterations.\n";
  $output .= " */ \n";
  $subtheme[$machine_name . '/' . 'template.php'] = $output;

  return $subtheme;
}
