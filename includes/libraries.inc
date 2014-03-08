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


