<?php

/**
 * @file
 * Kalatheme's subtheme setup stuff
 */

// Constants and stuff
define('DEFAULT_BOOTSTRAP_URL', 'http://github.com/twbs/bootstrap/releases/download/v3.0.3/bootstrap-3.0.3-dist.zip');
define('DEFAULT_BOOTSWATCH_URL', 'http://bootswatch.com');

/**
 * Form for kalatheme setup
 */
function kalatheme_setup_form($form, &$form_state) {
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
    '#description' => t('Upload any valid bootstrap library here to complete your install. For example: %filename from your local computer', array('%filename' => 'name.tar.gz')),
    // This #states rule says that the "file upload" field should only
    // be shown if the "bootstrap_library" form element is set to "custom".
    '#states' => array(
      'visible' => array(
        ':input[name="bootstrap_library"]' => array('value' => 'custom'),
      ),
    )
  );
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
  // Build basic subtheme
  $subtheme = _kalatheme_build_subtheme(addslashes($form_state['values']['subtheme_name']));

  // Extract library to tmp to prepare for STUFF
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

  // Find location of bootstap.css and bootstrap.min.css
  // @todo: something better?
  foreach ($files as $key => $value) {
    if (stripos($value, 'bootstrap.css') !== FALSE) {
      $bootstrap_css = $key;
    }
    if (stripos($value, 'bootstrap.min.css') !== FALSE) {
      $bootstrap_min_css = $key;
    }
  }
  // Customize Bootstrap CSS from Bootswatch if applicable
  if ($bootlib != 'custom' && $bootlib != 'default') {
    $bootstrap_file = system_retrieve_file(DEFAULT_BOOTSWATCH_URL . '/' . $bootlib . '/bootstrap.css' , $directory . DIRECTORY_SEPARATOR . $files[$bootstrap_css], FALSE, FILE_EXISTS_REPLACE);
    $bootstrap_file_min = system_retrieve_file(DEFAULT_BOOTSWATCH_URL . '/' . $bootlib . '/bootstrap.min.css', $directory . DIRECTORY_SEPARATOR . $files[$bootstrap_min_css], FALSE, FILE_EXISTS_REPLACE);
  }


  // Font awesome stuff goes here?
  // Write subtheme
  // Write bootstrap library
  // Enable subtheme
  // Redirect to apppearances page
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
  $output .= "description = A Kalatheme subtheme powered by Twitter Bootstrap & Compass. \n";
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
  $directory = &drupal_static(__FUNCTION__, '');
  if (empty($directory)) {
    $directory = 'temporary://kalatheme-' . $name . '-' . 'bootstrap-' . _kalatheme_unique_identifier();
    if ($create && !file_exists($directory)) {
      mkdir($directory);
    }
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
    // @todo: Do this in a way that doesn't suck and maybe incorporate some styles as seen on bootswatch?
    $form['bootstrap_preview']['#markup'] = t('<img src="@preview">', array('@preview' => 'http://bootswatch.com/' . $theme . '/thumbnail.png'));
  }
  return $form['bootstrap_preview'];
}
