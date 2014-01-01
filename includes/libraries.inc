<?php
/**
 * @file
 * Kalatheme's Bootstrap library definition
 */

/**
 * Implements hook_libraries_info().
 */
function kalatheme_libraries_info() {
  $libraries = array();
  $libraries[KALATHEME_BOOTSTRAP_LIBRARY] = array(
    'name' => 'Twitter Bootstrap',
    'machine name' => KALATHEME_BOOTSTRAP_LIBRARY,
    'vendor url' => 'http://twitter.github.com',
    'download url' => 'http://twitter.github.com',
    'path' => '',
    'version arguments' => array(
      'pattern' => '@v+([0-9a-zA-Z\.-]+)@',
      'lines' => 100,
      'cols' => 200,
    ),
    'version callback' => '_kalatheme_get_version',
    'versions' => array(
      '3' => array(
        'files' => array(
          'js' => array(
            'js/bootstrap.js',
          ),
          'css' => array(
            'css/bootstrap.css',
          ),
        ),
        'variants' => array(
          'minified' => array(
            'files' => array(
              'js' => array(
                'js/bootstrap.min.js',
              ),
              'css' => array(
                'css/bootstrap.min.css',
              ),
            ),
            'variant arguments' => array(
              'variant' => 'minified',
            ),
          ),
        ),
      ),
    ),
  );

  return $libraries;
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

/**
 * Gets the total column count from the Bootstrap grid in use.
 *
 * @return int
 *   The total number of columns.
 */
function kalatheme_get_grid_size() {
  // Check for cached result.
  $cached_size = cache_get('kalatheme_bootstrap_grid_size');
  if ($cached_size !== FALSE) {
    return $cached_size->data;
  }
  $size = _kalatheme_load_grid_size();
  cache_set('kalatheme_bootstrap_grid_size', $size, 'cache', CACHE_TEMPORARY);
  return $size;
}

/**
 * Parses the main Bootstrap CSS file to determine its grid size.
 */
function _kalatheme_load_grid_size() {
  $default_column_count = 12;
  // Load in the Bootstrap css file.
  $bootstrap_directory = libraries_get_path(KALATHEME_BOOTSTRAP_LIBRARY);
  if (!$bootstrap_directory) {
    return $default_column_count;
  }
  $bootstrap_file = $bootstrap_directory . '/css/bootstrap.min.css';
  if (!file_exists($bootstrap_file)) {
    $bootstrap_file = $bootstrap_directory . '/css/bootstrap.css';
    if (!file_exists($bootstrap_file)) {
      return $default_column_count;
    }
  }
  $bootstrap_contents = file_get_contents($bootstrap_file);
  // Find the largest column size.
  $column_sizes = array();
  preg_match_all('/col-md-(\d{1,2})/', $bootstrap_contents, $column_sizes);
  if (empty($column_sizes)) {
    return $default_column_count;
  }
  $column_sizes = array_pop($column_sizes);
  $column_sizes = array_unique($column_sizes);
  foreach ($column_sizes as &$column_size) {
    $column_size = (int)$column_size;
  }
  sort($column_sizes);

  return array_pop($column_sizes);
}
