<?php

/**
 * @file
 * Kalatheme's subtheme setup stuff
 */

// Constants and stuff
define('DEFAULT_BOOTSTRAP_URL', 'http://github.com/twbs/bootstrap/releases/download/v3.0.3/bootstrap-3.0.3-dist.zip');
define('DEFAULT_BOOTSWATCH_URL', 'http://bootswatch.com');
define('DEFAULT_FONTAWESOME_URL', 'http://fontawesome.io/assets/font-awesome-4.0.3.zip');

/**
 * Form for kalatheme setup
 */
function kalatheme_setup_form($form, &$form_state) {
  if (_kalatheme_backend_check()) {
    $form['backend_check'] = array(
      '#prefix' => '<div class="alert alert-success">',
      '#markup' => t("Your webserver is correctly configured for Kalatheme to build, install and enable your subtheme for you!"),
      '#suffix' => '</div>',
    );
  }
  else {
    $form['backend_check'] = array(
      '#prefix' => '<div class="alert alert-danger">',
      '#markup' => t("@backend_check", array('@backend_check' => _kalatheme_backend_check())),
      '#suffix' => '</div>',
    );
  }
  $form['help_text'] = array(
    '#prefix' => '<p>',
    '#markup' => t('Here you can easily setup a Kalatheme subtheme with the default or a custom Bootstrap library. For more information please consult our <a href="@setup_url">Start Up Guide</a> or just give it a whirl! The following file extensions are supported: %extensions.', array(
      '@setup_url' => 'http://drupal.org/project/kalatheme',
      '%extensions' => archiver_get_extensions(),
    )),
    '#suffix' => '</p>',
  );
  $form['subtheme_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name your theme'),
    '#description' => t('No special characters. For example: My Theme, Stuff'),
    '#required' => TRUE,
  );
  $form['bootstrap_library'] = array(
    '#type' => 'select',
    '#title' => t('Select a Bootstrap library.'),
    '#description' => t('Select a Bootstrap library to use with your subtheme. Visit <a href="@bootswatch">Bootswatch</a> for more info. Select "custom" to upload a third party Bootstrap library such as one from <a href="@wrapbootstrap">Wrapbootstrap</a> or built using Bootstrap\'s <a href="@bootcustom">Customization Utility</a>.', array(
      '@bootswatch' => 'http://bootswatch.com/',
      '@wrapbootstrap' => 'http://wrapbootstrap.com/',
      '@bootcustom' => 'http://getbootstrap.com/customize/',
    )),
    '#options' => array(
      'default' => t('Default'),
      'amelia' => t('Amelia'),
      'cerulean' => t('Cerulean'),
      'cosmo' => t('Cosmo'),
      'cyborg' => t('Cyborg'),
      'flatly' => t('Flatly'),
      'journal' => t('Journal'),
      'readable' => t('Readable'),
      'simplex' => t('Simplex'),
      'slate' => t('Slate'),
      'spacelab' => t('Spacelab'),
      'united' => t('United'),
      'yeti' => t('Yeti'),
      'custom' => t('Custom'),
    ),
    '#ajax' => array(
      'callback' => '_kalatheme_bootstrap_preview',
      'wrapper' => 'kalapreview-image-load-image',
      'method' => 'replace',
      'effect' => 'fade',
      'progress' => array('type' => 'none'),
    ),
    // '#required' => TRUE, don't need this because you have to select something!
  );
  $form['bootstrap_preview'] = array(
    '#prefix' => "<div id='kalapreview-image-load-image'>",
    '#suffix' => "</div>",
  );
  $form['bootstrap_upload'] = array(
    '#type' => 'file',
    '#title' => t('Custom Bootstrap library'),
    '#description' => t('Upload any <a href="@valid_doc">valid bootstrap library</a> here to complete your install. For example: %filename from your local computer. <strong>DISCLAIMER:</strong> This works pretty good with some libraries but not so great with others. Use this to kick start your project not as a end-all-be-all tool. ', array(
      '%filename' => 'name.tar.gz',
      '@valid_doc' => 'http://www.drupal.org/project/kalatheme',
    )),
    // This #states rule says that the "file upload" field should only
    // be shown if the "bootstrap_library" form element is set to "custom".
    '#states' => array(
      'visible' => array(
        ':input[name="bootstrap_library"]' => array('value' => 'custom'),
      ),
    )
  );
  $form['font_awesome'] = array(
    '#type' => 'checkbox',
    '#title' => t('Check this to include <a href="@font_awesome">Font Awesome</a> in your subtheme as well.', array(
      '@font_awesome' => 'http://fontawesome.io/',
    )),
    '#default_value' => TRUE,
  );
  if (_kalatheme_backend_check()) {
    $form['enable_theme'] = array(
      '#type' => 'checkbox',
      '#title' => t('Check this to enable the subtheme after it is built and installed.'),
      '#default_value' => TRUE,
    );
  }
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Set me up!'),
  );
  return $form;
}

/**
 * Form validation handler for kalatheme_setup_form().
 *
 * @see kalatheme_setup_form_submit()
 */
function kalatheme_setup_form_validate($form, &$form_state) {
  // Validate machine name
  $regex = '/^[\da-zA-Z _\']*$/';
  if (!preg_match($regex, $form_state['values']['subtheme_name'])) {
    form_set_error('subtheme_name', t('Please enter a valid machine name'));
  }

  // Make sure the user has uploaded an archive
  // @todo validate this is actually a bootstrap library
  if ($form_state['values']['bootstrap_library'] == 'custom') {
    if (empty($_FILES['files']['name']['bootstrap_upload'])) {
      form_set_error('bootstrap_upload', t('You must provide a custom Bootstrap library.'));
    }
  }
}

/**
 * Form submission handler for kalatheme_setup_form().
 *
 * Uploads the file attached to the form, then attempts to extract the archive
 * into a temporary location and verify it. Verifies the archive is a valid
 * Bootsrap library and if possible installs the library into the libraries API
 * directory. If not saves it for output to the user.
 *
 * Then builds a subtheme using the name provided and tries to install it. If
 * it cannot it grabs the bootstrap library and outputs both as an archive for
 * the user to install.
 *
 * @see kalatheme_setup_form_validate()
 */
function kalatheme_setup_form_submit($form, &$form_state) {
  // Let's borrow some stuff
  module_load_include('inc', 'update', 'update.manager');

  // Extract Bootstrap library to tmp to prepare for STUFF
  $bootlib = $form_state['values']['bootstrap_library'];
  if ($bootlib !== 'custom') {
    $local_cache = update_manager_file_get(DEFAULT_BOOTSTRAP_URL);
    if (!$local_cache) {
      form_set_error($field, t('Unable to retrieve Bootstrap from %url.', array('%url' => DEFAULT_BOOTSTRAP_URL)));
      return;
    }
  }
  else {
    if ($_FILES['files']['name']['bootstrap_upload']) {
      $validators = array('file_validate_extensions' => array(archiver_get_extensions()));
      $field = 'bootstrap_upload';
      if (!($finfo = file_save_upload($field, $validators, NULL, FILE_EXISTS_REPLACE))) {
        // Failed to upload the file. file_save_upload() calls form_set_error() on
        // failure.
        return;
      }
      $local_cache = $finfo->uri;
    }
  }
  $machine_name = strtolower(addslashes($form_state['values']['subtheme_name']));
  $machine_name = preg_replace('@[^a-z0-9_]+@', '_', $machine_name);
  $directory = _kalatheme_extract_directory($machine_name);
  try {
    $archive = update_manager_archive_extract($local_cache, $directory);
  }
  catch (Exception $e) {
    form_set_error($field, $e->getMessage());
    return;
  }
  $files = $archive->listContents();
  if (!$files) {
    form_set_error($field, t('Provided archive contains no files.'));
    return;
  }
  // Build basic Bootstrap library from input
  $bootstrap = _kalatheme_build_bootstrap($machine_name, $directory, $files);
  // Customize Bootstrap CSS from Bootswatch if applicable
  if ($bootlib != 'custom' && $bootlib != 'default') {
    $bootname = $machine_name . "_bootstrap";
    $css_dir = $bootname . DIRECTORY_SEPARATOR . "css" . DIRECTORY_SEPARATOR;
    $bootswatch = DEFAULT_BOOTSWATCH_URL . '/' . $bootlib;
    $bootstrap[$css_dir . 'bootstrap.css'] = file_get_contents(system_retrieve_file($bootswatch . '/bootstrap.css', 'temporary://bs.css', FALSE, FILE_EXISTS_REPLACE));
    $bootstrap[$css_dir . 'bootstrap.css'] = _kalatheme_lib_header() . $bootstrap[$css_dir . 'bootstrap.css'];
    $bootstrap[$css_dir . 'bootstrap.min.css'] = file_get_contents(system_retrieve_file($bootswatch . '/bootstrap.min.css', 'temporary://bm.css', FALSE, FILE_EXISTS_REPLACE));
    $bootstrap[$css_dir . 'bootstrap.min.css'] = _kalatheme_lib_header() . $bootstrap[$css_dir . 'bootstrap.min.css'];
  }

  // Build basic subtheme
  $subtheme = _kalatheme_build_subtheme(addslashes($form_state['values']['subtheme_name']));
  if ($form_state['values']['font_awesome']) {
    if (!_kalatheme_build_fontawesome($machine_name, $subtheme)) {
      return;
    }
  }
  // Get other files from custom bootsrap library for subtheme
  if ($bootlib == 'custom') {
    if (!_kalatheme_build_custom_stuff($machine_name, $directory, $files, $subtheme)) {
      return;
    }
  }

  // Try to write the subtheme and bootstrap library to tmp
  if (!_kalatheme_write_subtheme($subtheme) || !_kalatheme_write_bootstrap($bootstrap)) {
    form_set_error('bootstrap_library', t('Could not write stuff to tmp. That is weird.'));
    return;
  }
  // Move from temp to legitness or output to the user for DL
  $booted = TRUE;
  $theme_path = drupal_realpath(substr(drupal_get_path('theme', 'kalatheme'), 0, strrpos(drupal_get_path('theme', 'kalatheme'), '/')));
  $library_path = drupal_realpath('sites/all/libraries');
  $stuff = array(
    sys_get_temp_dir() . DIRECTORY_SEPARATOR . $machine_name =>  $theme_path . DIRECTORY_SEPARATOR . $machine_name,
    sys_get_temp_dir() . DIRECTORY_SEPARATOR . $machine_name . '_bootstrap' =>  $library_path . DIRECTORY_SEPARATOR . $machine_name . '_bootstrap',
  );
  if (_kalatheme_backend_check()) {
    // Move subtheme and bootstrap out of temp and into drupal places else
    // present it to user for download
    // @todo: there has to be a better way?
    foreach ($stuff as $source => $dest) {
      foreach ($iterator = new RecursiveIteratorIterator(new RecursiveDirectoryIterator($source, RecursiveDirectoryIterator::SKIP_DOTS), RecursiveIteratorIterator::SELF_FIRST) as $item) {
        if (is_dir($dest) || mkdir($dest, 0777, TRUE)) {
          if ($item->isDir()) {
            if (!is_dir($dest . DIRECTORY_SEPARATOR . $iterator->getSubPathName())) {
              if (!mkdir($dest . DIRECTORY_SEPARATOR . $iterator->getSubPathName())) {
                $booted = FALSE;
              }
            }
          }
          else {
            if (!copy($item, $dest . DIRECTORY_SEPARATOR . $iterator->getSubPathName())) {
              $booted = FALSE;
            }
          }
        }
        else {
          $booted = FALSE;
        }
      }
    }
  }
  else {
    $zip = new ZipArchive();
    $zip_name = drupal_realpath('public://') . DIRECTORY_SEPARATOR . $machine_name . ".zip";
    $zip->open($zip_name,  ZipArchive::CREATE);
    foreach ($stuff as $source => $dest) {
      foreach ($iterator = new RecursiveIteratorIterator(new RecursiveDirectoryIterator($source, RecursiveDirectoryIterator::SKIP_DOTS), RecursiveIteratorIterator::SELF_FIRST) as $item) {
        if (!$item->isDir()) {
          $filepath = $source . DIRECTORY_SEPARATOR . $iterator->getSubPathName();
          $zip->addFile($filepath, str_replace(sys_get_temp_dir(), '/kalasubtheme', $filepath));
        }
      }
    }
    $zip->close();
    drupal_goto(variable_get('file_public_path') . DIRECTORY_SEPARATOR . $machine_name . ".zip");
    return;
  }

  // Enable theme if applicable
  $message = '<i class="fa fa-thumbs-up fa-2x">Your new theme is installed but not yet enabled. It should now appear below.</i>';
  if ($form_state['values']['enable_theme'] && $booted) {
    system_rebuild_theme_data();
    drupal_theme_rebuild();
    theme_enable(array($machine_name));
    variable_set('theme_default', $machine_name);
    $message = '<i class="fa fa-thumbs-up fa-2x">Your new subtheme is enabled! Looking good in the neighborhood!!</i>';
  }

  // Not the best but we want the user to see their new stuff.
  cache_clear_all();

  // Kick it back
  drupal_set_message(t($message), 'status', FALSE);
  $form_state['redirect'] = 'admin/appearance';
}

/**
 * Construct an array of subtheme things.
 *
 * @param  string $name The subtehme name
 * @return array  $subtheme An array containng the files for the subtheme
 *
 */
function _kalatheme_build_subtheme($name) {
  $subtheme = array();
  $machine_name = strtolower($name);
  $machine_name = preg_replace('@[^a-z0-9_]+@', '_', $machine_name);

  // Basic Info File
  $output  = "name = " . $name . " \n";
  $output .= "description = A Kalatheme subtheme powered by Twitter Bootstrap & Science. \n";
  $output .= "base theme = kalatheme \n";
  $output .= "core = 7.x \n";
  $output .= "\n";
  $output .= "regions[content] = Content \n";
  $output .= "\n";
  $output .= "stylesheets[all][] = css/main.css \n";
  $output .= "\n";
  $subtheme[$machine_name . DIRECTORY_SEPARATOR . $machine_name . '.info'] = $output;

  // Basic CSS File
  $output  = "/*! \n";
  $output .= " * Kalatheme (http://drupal.org/project/kalatheme) \n";
  $output .= " */ \n";
  $subtheme[$machine_name . DIRECTORY_SEPARATOR . 'css' . DIRECTORY_SEPARATOR . 'main.css'] = $output;

  return $subtheme;
}

/**
 * Translate a subtheme array into writable files.
 *
 * @param  array  $subtheme The subtheme stuff
 * @return boolean TRUE is for success, FALSE is for fail
 *
 */
function _kalatheme_write_subtheme($subtheme) {
  foreach ($subtheme as $key => $value) {
    $dir = substr($key, 0, strrpos($key, '/'));
    if (is_dir('temporary://' . $dir) || mkdir('temporary://' . $dir, 0777, TRUE)) {
      if (!file_put_contents('temporary://' . $key, $value)) {
        return FALSE;
      }
    }
    else {
      return FALSE;
    }
  }
  return TRUE;
}

/**
 * Move the bootstrap library from tmp to sites/all/libraries
 *
 * @param  string  $bootlib The machine name of the subtheme
 * @param  string  $directory The location of the temp bootstrap lib
 * @param  array   $files An array of the bootstral files
 * @return boolean TRUE is for success, FALSE is for fail
 *
 */
function _kalatheme_build_bootstrap($machine_name, $directory, $files) {
  $bootname = $machine_name . "_bootstrap";
  $css_dir = $bootname . DIRECTORY_SEPARATOR . "css" . DIRECTORY_SEPARATOR;
  $js_dir = $bootname . DIRECTORY_SEPARATOR . "js" . DIRECTORY_SEPARATOR;
  $fonts_dir = $bootname . DIRECTORY_SEPARATOR . "fonts" . DIRECTORY_SEPARATOR;
  $bootstrap = array();
  // @todo: Gotta be a sexier way to do this?
  foreach ($files as $key => $value) {
    // CSS
    if (stripos($value, 'bootstrap.css') !== FALSE) {
      $bootstrap[$css_dir . 'bootstrap.css'] = file_get_contents($directory . DIRECTORY_SEPARATOR . $value);
    }
    if (stripos($value, 'bootstrap.min.css') !== FALSE) {
      $bootstrap[$css_dir . 'bootstrap.min.css'] = file_get_contents($directory . DIRECTORY_SEPARATOR . $value);
    }
    // JS
    if (stripos($value, 'bootstrap.js') !== FALSE) {
      $bootstrap[$js_dir . 'bootstrap.js'] = file_get_contents($directory . DIRECTORY_SEPARATOR . $value);
    }
    if (stripos($value, 'bootstrap.min.js') !== FALSE) {
      $bootstrap[$js_dir . 'bootstrap.min.js'] = file_get_contents($directory . DIRECTORY_SEPARATOR . $value);
    }
    // FONTS
    if (stripos($value, 'glyphicons-halflings-regular.woff') !== FALSE) {
      $bootstrap[$fonts_dir . 'glyphicons-halflings-regular.woff'] = file_get_contents($directory . DIRECTORY_SEPARATOR . $value);
    }
    if (stripos($value, 'glyphicons-halflings-regular.ttf') !== FALSE) {
      $bootstrap[$fonts_dir . 'glyphicons-halflings-regular.ttf'] = file_get_contents($directory . DIRECTORY_SEPARATOR . $value);
    }
    if (stripos($value, 'glyphicons-halflings-regular.svg') !== FALSE) {
      $bootstrap[$fonts_dir . 'glyphicons-halflings-regular.svg'] = file_get_contents($directory . DIRECTORY_SEPARATOR . $value);
    }
    if (stripos($value, 'glyphicons-halflings-regular.eot') !== FALSE) {
      $bootstrap[$fonts_dir . 'glyphicons-halflings-regular.eot'] = file_get_contents($directory . DIRECTORY_SEPARATOR . $value);
    }
  }
  return $bootstrap;
}

/**
 * Translate a bootstrap array into writable files.
 *
 * @param  array  $bootstrap The bootstrap stuff
 * @return boolean TRUE is for success, FALSE is for fail
 *
 */
function _kalatheme_write_bootstrap($bootstrap) {
  foreach ($bootstrap as $key => $value) {
    $dir = substr($key, 0, strrpos($key, '/'));
    if (is_dir('temporary://' . $dir) || mkdir('temporary://' . $dir, 0777, TRUE)) {
      if (!file_put_contents('temporary://' . $key, $value)) {
        return FALSE;
      }
    }
    else {
      return FALSE;
    }
  }
  return TRUE;
}

/**
 * Build fontawesome support into subtheme
 *
 * @param  string  $subtheme The array of subtheme things
 * @return boolean TRUE is for success, FALSE is for fail
 *
 */
function _kalatheme_build_fontawesome($machine_name, &$subtheme) {
  $local_cache = update_manager_file_get(DEFAULT_FONTAWESOME_URL);
  if (!$local_cache) {
    form_set_error($field, t('Unable to retrieve Bootstrap from %url.', array('%url' => DEFAULT_BOOTSTRAP_URL)));
    return FALSE;
  }
  $directory = _kalatheme_extract_directory('font_awesome');
  try {
    $archive = update_manager_archive_extract($local_cache, $directory);
  }
  catch (Exception $e) {
    form_set_error($field, $e->getMessage());
    return FALSE;
  }
  $files = $archive->listContents();
  if (!$files) {
    form_set_error($field, t('Provided archive contains no files.'));
    return FALSE;
  }
  foreach ($files as $file) {
    if (strpos($file, '.css') !== FALSE && !is_dir($directory . DIRECTORY_SEPARATOR . $file)) {
      $subtheme[$machine_name . DIRECTORY_SEPARATOR . 'css' . substr($file, strrpos($file, '/'))] = file_get_contents($directory . DIRECTORY_SEPARATOR . $file);
    }
    if (strpos($file, '/fonts/') !== FALSE && !is_dir($directory . DIRECTORY_SEPARATOR . $file)) {
      $subtheme[$machine_name . DIRECTORY_SEPARATOR . 'fonts' . substr($file, strrpos($file, '/'))] = file_get_contents($directory . DIRECTORY_SEPARATOR . $file);
    }
  }
  $output  = "stylesheets[all][] = css/font-awesome.min.css \n";
  $output .= "\n";
  $subtheme[$machine_name . DIRECTORY_SEPARATOR . $machine_name . '.info'] .= $output;
  return $subtheme;
}

/**
 * Deep magic that goes through a valid custom bootstrap library and tries to
 * add in the extra stuff files correctly
 *
 * @param string $machine_name the name of the subtheme
 * @param string $directory    the location of the files on disk
 * @param array  $files        the files in the custom library
 * @param array  $subtheme     the subtheme to be written
 *
 */
function _kalatheme_build_custom_stuff($machine_name, $directory, $files, &$subtheme) {
  // Get rid of all the bullshit
  foreach ($files as $index => $file) {
    if (_kalatheme_strip_types($file)) {
      unset($files[$index]);
    }
  }
  foreach ($files as $index => $file) {
    if (_kalatheme_strip_files($file)) {
      unset($files[$index]);
    }
  }
  // For now we are going to focus on splitting the rest of the files up into
  // asset classes. We might want to either abstract these functions in the
  // future or add more asset classes.
  $stuff = array(
    'css/' => 'stylesheets[all][]',
    'js/'  => 'scripts[]',
    'img/' => NULL,
  );
  foreach ($stuff as $type => $info_prefix) {
    $type_dir = $machine_name . DIRECTORY_SEPARATOR . $type;
    foreach ($files as $index => $file) {
      if (strpos($file, $type) !== FALSE) {
        $type_file = substr($file, strrpos($file, $type));
        $type_file = str_replace($type, '', $type_file);
        $subtheme[$type_dir . $type_file] = file_get_contents($directory . DIRECTORY_SEPARATOR . $file);
        if ($info_prefix) {
          $subtheme[$machine_name . DIRECTORY_SEPARATOR . $machine_name . '.info'] .= $info_prefix . " = " . $type . $type_file . " \n";
        }
      }
    }
  }
  return $subtheme;
}

/**
 * Prune custom library, removing types we don't want
 */
function _kalatheme_strip_types($file) {
  $wanted_types = array('.css', '.js', '.png', '.jpg');
  foreach ($wanted_types as $type) {
    if (stripos($file, $type) !== FALSE) {
      return FALSE;
    }
  }
  return TRUE;
}

/**
 * Prune custom library, removing files we don't want
 */
function _kalatheme_strip_files($file) {
  // @todo might want to update this over time?
  $unwanted_files = array('bootstrap', 'jquery.js', 'jquery.min.js');
  foreach ($unwanted_files as $type) {
    if (stripos($file, $type) !== FALSE) {
      return TRUE;
    }
  }
  return FALSE;
}

/**
 * Returns a short unique identifier for this Drupal installation.
 *
 * @return
 *   An eight character string uniquely identifying this Drupal installation.
 */
function _kalatheme_unique_identifier() {
  $id = &drupal_static(__FUNCTION__, '');
  if (empty($id)) {
    $id = substr(hash('sha256', drupal_get_hash_salt()), 0, 8);
  }
  return $id;
}

/**
 * Returns the directory where update archive files should be extracted.
 *
 * @param $create
 *   (optional) Whether to attempt to create the directory if it does not
 *   already exist. Defaults to TRUE.
 *
 * @return
 *   The full path to the temporary directory where update file archives should
 *   be extracted.
 */
function _kalatheme_extract_directory($name, $create = TRUE) {
  $directory = 'temporary://kalatheme-' . $name . '-' . 'bootstrap-' . _kalatheme_unique_identifier();
  if ($create && !file_exists($directory)) {
    mkdir($directory);
  }
  return $directory;
}

/**
 * Ajax callback that returns HTML to the setup form so that the user
 * can see a preview of the bootstrap library they are selecting.
 *
 * @param  [type] $form       Form
 * @param  [type] $form_state Form state
 *
 * @return [type]             Updated form state to be merged back in
 */
function _kalatheme_bootstrap_preview($form, &$form_state) {
  $theme = $form_state['values']['bootstrap_library'];
  if ($theme == 'default' || $theme == 'custom') {
    $form['bootstrap_preview']['#markup'] = t('');
  }
  else {
    // @todo: Do this in a way that doesn't suck and maybe incorporate some
    // styles as seen on bootswatch?
    $form['bootstrap_preview']['#markup'] = t('<img src="@preview">', array('@preview' => 'http://bootswatch.com/' . $theme . '/thumbnail.png'));
  }
  return $form['bootstrap_preview'];
}

/**
 * We want to check if the webserver can write our stuff to the right places.
 * If it can't then we are going to tell the user what's up.
 *
 * @return [mixed] True if all good, message if no good
 */
function _kalatheme_backend_check() {
  $theme_path = drupal_realpath(substr(drupal_get_path('theme', 'kalatheme'), 0, strrpos(drupal_get_path('theme', 'kalatheme'), '/')));
  $library_path = drupal_realpath('sites/all/libraries');
  // Create paths if needed
  // $theme_path should already exist
  if (!$library_path) {
    if (!mkdir(DRUPAL_ROOT . "/sites/all/libraries", 0777, TRUE)) {
      return t("If you want Kalatheme to be able to install and enable your subtheme for you please make sure that both @lib_path and @theme_path exist and are writable. Otherwise it will just give you an achive to download.", array(
        '@lib_path' => $library_path,
        '@theme_path' => $theme_path,
      ));;
    }
    $library_path = drupal_realpath('sites/all/libraries');
  }
  // Check if directories are raedy to receive a payload
  // @todo this needs a lot more testing, always seems to be true on kalabox
  if (!file_prepare_directory($theme_path) || !file_prepare_directory($library_path)) {
    return t("If you want Kalatheme to be able to install and enable your subtheme for you please make sure that both @lib_path and @theme_path exist and are writable. Otherwise it will just give you an achive to download.", array(
      '@lib_path' => $library_path,
      '@theme_path' => $theme_path,
    ));;
  }
  return TRUE;
}

/**
 * Returns some stuff to append to the top of Bootswatch libraries because
 * we need version info for the libraries API
 */
function _kalatheme_lib_header() {
  $output  = "/*! \n";
  $output .= " * Bootstrap v3.0.0 \n";
  $output .= " */ \n";
  return $output;
}
