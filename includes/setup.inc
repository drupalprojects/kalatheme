<?php

/**
 * @file
 * Kalatheme's subtheme setup stuff
 */

// Constants and stuff
define('DEFAULT_BOOTSTRAP_URL', 'https://github.com/twbs/bootstrap/releases/download/v3.1.0/bootstrap-3.1.0-dist.zip');
define('DEFAULT_BOOTSWATCH_URL', 'http://bootswatch.com');
define('DEFAULT_FONTAWESOME_URL', 'http://fontawesome.io/./assets/font-awesome-4.0.3.zip');

/**
 * Form for kalatheme setup
 */
function kalatheme_setup_form() {
  require_once DRUPAL_ROOT . '/' . drupal_get_path('theme', 'kalatheme') . '/includes/utils.inc';

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
      '#markup' => t("If you want Kalatheme to be able to install and enable your subtheme for you please make sure that both @lib_path and @theme_path exist and are writable. Otherwise it will just give you an archive to download.", array(
        '@lib_path' => is_dir('sites/all/libraries') ? 'sites/all/libraries' : 'sites/all',
        '@theme_path' => drupal_realpath(substr(drupal_get_path('theme', 'kalatheme'), 0, strrpos(drupal_get_path('theme', 'kalatheme'), '/'))),
      )),
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

  $base_themes = _kalatheme_get_base_themes();

  if (count($base_themes) > 1) {
    $form['subtheme_base'] = array(
      '#type' => 'select',
      '#title' => t('Subtheme base'),
      '#description' => t('The base theme to use for your theme.'),
      '#required' => TRUE,
      '#options' => $base_themes,
      '#default_value' => 'kalatheme',
    );
  }
  else {
    $form['subtheme_base'] = array(
      '#type' => 'hidden',
      '#value' => 'kalatheme',
    );
  }

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
    '#value' => t('Dress me up!'),
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
  $regex = '/^[a-zA-Z][\da-zA-Z _\']*$/';
  if (!preg_match($regex, $form_state['values']['subtheme_name'])) {
    form_set_error('subtheme_name', t('Please enter a valid machine name.'));
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
 * Uses BATCH API for file processing and HTTP request stuffs
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
  // Let's begin to define some opps
  $operations = array();
  $batch = array(
    'finished' => '_kalatheme_batch_done',
    'title' => t('Kalasubthemeing...'),
    'init_message' => t('Starting.'),
    'progress_message' => t('Kalathemed @current of @total in @elapsed'),
    'error_message' => t('Kalasubthemeing has resulted in the unspeakable:'),
    'file' => drupal_get_path('theme', 'kalatheme') . '/includes/setup.inc',
  );
  // Grab some vars
  $enable = (isset($form_state['values']['enable_theme'])) ? $form_state['values']['enable_theme'] : FALSE;
  $machine_name = strtolower(addslashes($form_state['values']['subtheme_name']));
  $machine_name = preg_replace('@[^a-z0-9_]+@', '_', $machine_name);
  $bootlib = $form_state['values']['bootstrap_library'];
  // Handle processing of URL vs upload
  if ($bootlib === 'custom' && $_FILES['files']['name']['bootstrap_upload']) {
    $validators = array('file_validate_extensions' => array(archiver_get_extensions()));
    $field = 'bootstrap_upload';
    $finfo = file_save_upload($field, $validators, NULL, FILE_EXISTS_REPLACE);
    $local_cache = $finfo->uri;
    $operations[] = array('_kalatheme_get_bootstrap', array($local_cache, $machine_name, $enable));
  }
  else {
    $operations[] = array('_kalatheme_get_bootstrap', array(FALSE, $machine_name, $enable));
  }
  $operations[] = array('_kalatheme_build_bootstrap', array($machine_name, FALSE, FALSE));
  // Customize Bootstrap CSS from Bootswatch if applicable
  if ($bootlib != 'custom' && $bootlib != 'default') {
    $files = array('bootstrap.css', 'bootstrap.min.css');
    foreach ($files as $file) {
      $operations[] = array('_kalatheme_get_bootswatch', array($bootlib, $machine_name, $file));
    }
  }
  // Build basic subtheme
  $subtheme_params = array(
    'name' => addslashes($form_state['values']['subtheme_name']),
    'base' => $form_state['values']['subtheme_base'],
  );
  $operations[] = array('_kalatheme_build_subtheme', array($subtheme_params));
  // Get FontAwesome
  if ($form_state['values']['font_awesome']) {
    $operations[] = array('_kalatheme_get_fontawesome', array($machine_name));
    $operations[] = array('_kalatheme_build_fontawesome', array($machine_name, FALSE, FALSE));
  }
  // Get other files from custom bootsrap library for subtheme
  if ($bootlib === 'custom') {
    $operations[] = array('_kalatheme_build_custom_stuff', array($machine_name, FALSE, FALSE));
  }
  // Dress you up in my love
  $operations[] = array('_kalatheme_dress_you_up', array($machine_name));
  // Build the batch and cook it
  $batch['operations'] = $operations;
  batch_set($batch);
}

/**
 * Gets Bootstrap from file upload or the tubes. Sets some other things to be
 * used later on in the batch.
 *
 * @param $file
 *   The location of Bootstrip lib if uploaded
 * @param $machine_name
 *   The machine name of the subtheme
 * @param $files
 *   Enable the theme at end of processing or not
 * @param $context
 *   Batch things
 */
function _kalatheme_get_bootstrap($upload = FALSE, $machine_name, $enable, &$context) {
  $context['results']['machine_name'] = $machine_name;
  $context['results']['enable'] = $enable;
  // Let's borrow some stuff
  module_load_include('inc', 'update', 'update.manager');
  $context['message'] = t('Getting Bootstrap Library');
  // Extract Bootstrap library to tmp to prepare for STUFF
  // @todo: we might want to move this over to the file_put_contents treatment
  // we are doing for bootswatch and fontawesome at some point
  $local_file = ($upload == FALSE) ? update_manager_file_get(DEFAULT_BOOTSTRAP_URL) : $upload;
  $context['results']['bootdir'] = _kalatheme_extract_directory($machine_name);
  $archive = update_manager_archive_extract($local_file, $context['results']['bootdir']);
  // Verify the library, kind of
  $files = $archive->listContents();
  $valid = FALSE;
  // Check that it contains needed files
  foreach ($files as $file) {
    if (stripos($file, 'bootstrap.min.css') !== FALSE || stripos($file, 'bootstrap.css') !== FALSE) {
      $check_file = $file;
      $valid = TRUE;
      break;
    }
  }
  // Check that needed file exists
  if (!file_exists($context['results']['bootdir'] . DIRECTORY_SEPARATOR . $check_file)) {
    $valid = FALSE;
  }
  if (!$valid) {
    // @todo some sort of dyin and reportin
  }
  else {
    $context['results']['bootfiles'] = $files;
  }
}

/**
 * Builds an array mimicing the Bootstrap archive
 *
 * @param $machine_name
 *   The machine name of the subtheme
 * @param $directory
 *   The location of the temp bootstrap lib
 * @param $files
 *   An array of files
 * @param $context
 *   Batch things
 *
 */
function _kalatheme_build_bootstrap($machine_name, $directory = FALSE, $files = FALSE, &$context) {
  $context['message'] = t('Building Bootstrap Library');
  // Get results if in batch and not passed in
  if (!$directory && isset($context['results']['bootdir'])) {
    $directory = $context['results']['bootdir'];
  }
  if (!$files && isset($context['results']['bootfiles'])) {
    $files = $context['results']['bootfiles'];
  }
  // Define some thing for better readables
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
  $context['results']['bootstrap'] = $bootstrap;
}

/**
 * Gets Bootswatch CSS files and overwrites the defaults
 *
 * @param $bootlib
 *   The machine name of the subtheme
 * @param $machine_name
 *   The machine name of the subtheme
 * @param $file
 *   File to be added
 * @param $context
 *   Batch things
 *
 */
function _kalatheme_get_bootswatch($bootlib, $machine_name, $file, &$context) {
  // Let's borrow some stuff
  $context['message'] = t('Customizing Bootstrap Library');
  $bootname = $machine_name . "_bootstrap";
  $css_dir = $bootname . DIRECTORY_SEPARATOR . "css" . DIRECTORY_SEPARATOR;
  $bootswatch = DEFAULT_BOOTSWATCH_URL . '/' . $bootlib . '/';
  $local_file = 'temporary://' . $file;
  $trying = TRUE;
  // Try it until it works
  while ($trying) {
    $download = file_put_contents($local_file, fopen($bootswatch . $file, 'r'));
    // We've got a good DL
    if ($download != FALSE) {
      // Check to make sure we actually got something
      if (file_exists($local_file)) {
        // Overwrite existing bootstrap file
        // Add a header with bootstrap 3 info so it is recongized by libraies API
        // Do some file clean up
        // Remove error messages that may have accrued in previous attempts
        $context['results']['bootstrap'][$css_dir . $file] = _kalatheme_lib_header() . file_get_contents($local_file);
        unlink($local_file);
        // @todo might want to consider something else here?
        $trying = FALSE;
      }
    }
  }
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
function _kalatheme_build_subtheme($params, &$context) {
  $name = $params['name'];
  $base = $params['base'];
  $context['message'] = t('Building Subtheme');
  $subtheme = array();
  $machine_name = strtolower($name);
  $machine_name = preg_replace('@[^a-z0-9_]+@', '_', $machine_name);
  // Basic Info File
  $output  = "name = " . $name . " \n";
  $output .= "description = A Kalatheme subtheme powered by Twitter Bootstrap & Science. \n";
  $output .= "base theme = " . $base . " \n";
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
  $context['results']['subtheme'] = $subtheme;
}

/**
 * Download the FontAwesome library
 *
 * @param $machine_name
 *   The machine name of the subtheme
 * @param $context
 *   Batch things
 *
 */
function _kalatheme_get_fontawesome($machine_name, &$context) {
  $context['message'] = t('Downloading FontAwesome');
  // Let's borrow some stuff
  module_load_include('inc', 'update', 'update.manager');
  // For some strange reason system_file_retrieve throws up when trying to grab
  // this archive so we are going to take it lower
  $local_file = 'temporary://font-awesome.zip';
  $download = file_put_contents($local_file, fopen(DEFAULT_FONTAWESOME_URL, 'r'));
  if ($download != FALSE) {
    $context['results']['fontdir'] = _kalatheme_extract_directory('font_awesome');
    $archive = update_manager_archive_extract($local_file, $context['results']['fontdir']);
    // Do some basic validation
    $files = $archive->listContents();
    $valid = FALSE;
    // Check that it contains needed files
    foreach ($files as $file) {
      if (stripos($file, 'font-awesome.min.css') !== FALSE || stripos($file, 'FontAwesome.otf') !== FALSE) {
        $check_file = $file;
        $valid = TRUE;
        break;
      }
    }
    // Check that needed file exists
    if (!file_exists($context['results']['fontdir'] . DIRECTORY_SEPARATOR . $check_file)) {
      $valid = FALSE;
    }
    if (!$valid) {
      // @todo some sort of dyin and reportin
    }
    else {
      $context['results']['fontfiles'] = $files;
    }
    unlink($local_file);
  }
}

/**
 * Build FontAwesome into the subtheme
 *
 * @param $machine_name
 *   The machine name of the subtheme
 * @param $directory
 *   The location of the temp bootstrap lib
 * @param $files
 *   An array of files
 * @param $context
 *   Batch things
 *
 */
function _kalatheme_build_fontawesome($machine_name, $directory = FALSE, $files = FALSE, &$context) {
  $context['message'] = t('Adding FontAwesome to your subtheme');
  // Get results if in batch and not passed in
  if (!$directory && isset($context['results']['fontdir'])) {
    $directory = $context['results']['fontdir'];
  }
  if (!$archive && isset($context['results']['fontfiles'])) {
    $files = $context['results']['fontfiles'];
  }
  foreach ($files as $file) {
    if (strpos($file, '.css') !== FALSE && !is_dir($directory . DIRECTORY_SEPARATOR . $file)) {
      $context['results']['subtheme'][$machine_name . DIRECTORY_SEPARATOR . 'css' . substr($file, strrpos($file, '/'))] = file_get_contents($directory . DIRECTORY_SEPARATOR . $file);
    }
    if (strpos($file, '/fonts/') !== FALSE && !is_dir($directory . DIRECTORY_SEPARATOR . $file)) {
      $context['results']['subtheme'][$machine_name . DIRECTORY_SEPARATOR . 'fonts' . substr($file, strrpos($file, '/'))] = file_get_contents($directory . DIRECTORY_SEPARATOR . $file);
    }
  }
  $output  = "stylesheets[all][] = css/font-awesome.min.css \n";
  $output .= "\n";
  $context['results']['subtheme'][$machine_name . DIRECTORY_SEPARATOR . $machine_name . '.info'] .= $output;
}

/**
 * Deep magic that goes through a valid custom bootstrap library and tries to
 * add in the extra stuff files correctly
 *
 * @param $machine_name
 *   The machine name of the subtheme
 * @param $directory
 *   The location of the temp bootstrap lib
 * @param $archive
 *   An archive object of files
 * @param $context
 *   Batch things
 *
 */
function _kalatheme_build_custom_stuff($machine_name, $directory = FALSE, $archive = FALSE, &$context) {
  $context['message'] = t('Pulling in custom stuff');
  // Get results if in batch and not passed in
  if (!$directory && isset($context['results']['bootdir'])) {
    $directory = $context['results']['bootdir'];
  }
  if (!$files && isset($context['results']['bootfiles'])) {
    $files = $context['results']['bootfiles'];
  }
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
        if (!isset($context['results']['subtheme'][$type_dir . $type_file])) {
          $context['results']['subtheme'][$type_dir . $type_file] = file_get_contents($directory . DIRECTORY_SEPARATOR . $file);
          if ($info_prefix) {
            $context['results']['subtheme'][$machine_name . DIRECTORY_SEPARATOR . $machine_name . '.info'] .= $info_prefix . " = " . $type . $type_file . " \n";
          }
        }
      }
    }
  }
}

/**
 * Complete the subtheme construction by either writing the subtheme and library
 * to the correct location of providing a download link of the archive. Enables
 * the theme and sets it to the default theme if option seleted.
 *
 * @param $machine_name
 *   The machine name of the subtheme
 * @param $context
 *   Batch things
 */
function _kalatheme_dress_you_up($machine_name, &$context) {
  $context['message'] = t('Dress you up in my love');
  // Get results if in batch and not passed in
  // Try to write the subtheme and bootstrap library to tmp
  if (!_kalatheme_write_stuff($context['results']['subtheme']) || !_kalatheme_write_stuff($context['results']['bootstrap'])) {
    // @todo: Help
  }
  // Move from temp to legitness or output to the user for DL
  $booted = TRUE;
  $theme_path = drupal_realpath(substr(drupal_get_path('theme', 'kalatheme'), 0, strrpos(drupal_get_path('theme', 'kalatheme'), '/')));
  $library_path = drupal_realpath('sites/all/libraries');
  $stuff = array(
    file_directory_temp() . DIRECTORY_SEPARATOR . $machine_name =>  $theme_path . DIRECTORY_SEPARATOR . $machine_name,
    file_directory_temp() . DIRECTORY_SEPARATOR . $machine_name . '_bootstrap' =>  $library_path . DIRECTORY_SEPARATOR . $machine_name . '_bootstrap',
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
    $booted = FALSE;
  }
  // Enable theme if applicable
  if ($context['results']['enable'] && $booted) {
    system_rebuild_theme_data();
    drupal_theme_rebuild();
    theme_enable(array($machine_name));
    variable_set('theme_default', $machine_name);
    $context['results']['message'] = '<i class="fa fa-thumbs-up fa-2x">Your new subtheme is enabled! Looking good in the neighborhood!!</i>';
    cache_clear_all();
  }
  elseif (!$context['results']['enable'] && $booted) {
    $context['results']['message'] = '<i class="fa fa-thumbs-up fa-2x">Your new theme is installed but not yet enabled. It should now appear below.</i>';
  }
  else {
    $context['results']['message'] = t('We built your subtheme but you will need to install it <a href="@setup_url">manually</a>. You can download your new subtheme <a href="@download_link">here.</a>' , array(
      '@setup_url' => 'http://drupal.org/project/kalatheme',
      '@download_link' => url(variable_get('file_public_path') . DIRECTORY_SEPARATOR . $machine_name . ".zip"),
    ));
  }
}

/**
 * Translate a subtheme array into writable files.
 *
 * @param $stuff
 *   Array of stuff to write
 *
 * @return
 *   TRUE is for success, FALSE is for fail
 *
 */
function _kalatheme_write_stuff($stuff) {
  foreach ($stuff as $key => $value) {
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
 * Clean up stuff post batch.
 */
function _kalatheme_batch_done($success, $results, $operations) {
  // Kick it back
  if ($success) {
    system_rebuild_theme_data();
    drupal_theme_rebuild();
    cache_clear_all();
    // We do this to remove the "bootstrap not found error"
    // @todo something better than this ?
    drupal_get_messages('error', TRUE);
    // Show something good?
    drupal_set_message(t($results['message']), 'status', FALSE);
  }
  else {
    // @todo need to provide better error handling before this really is
    // relevant
    drupal_set_message(t($results['message']), 'warning', FALSE);
  }
  drupal_goto('admin/appearance');
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
 * @param $form
 *   Form
 * @param $form_state
 *   Form state
 *
 * @return
 *   Updated form state to be merged back in
 *
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
 * @return
 *   True if all good, message if no good
 */
function _kalatheme_backend_check() {
  $theme_dir = drupal_realpath(substr(drupal_get_path('theme', 'kalatheme'), 0, strrpos(drupal_get_path('theme', 'kalatheme'), '/')));
  $libraries_dir = 'sites/all/libraries';
  // Create paths if needed
  // $theme_path should already exist
  // This is probably redundant since file_prepare_directory does this too
  if (!is_dir($libraries_dir)) {
    mkdir(DRUPAL_ROOT . "/sites/all/libraries", 0777, TRUE);
  }
  // Check if directories are raedy to receive a payload
  // @todo this needs a lot more testing, always seems to be true on kalabox
  if (!file_prepare_directory($libraries_dir) || !file_prepare_directory($theme_dir)) {
    // @todo something
  }
  return is_writable(drupal_realpath($libraries_dir)) && is_writable($theme_dir);
}

/**
 * Returns some stuff to append to the top of Bootswatch libraries because
 * we need version info for the libraries API
 *
 * @return
 *   Basic header to go on custom libraries so they are detected properly
 */
function _kalatheme_lib_header() {
  // @todo get this to actually report the correct version?
  $output  = "/*! \n";
  $output .= " * Bootstrap v3.0.0 \n";
  $output .= " */ \n";
  return $output;
}
