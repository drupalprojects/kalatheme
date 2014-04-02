<?php
/**
 * @file
 * Kalatheme's subtheme config form stuff
 */

/**
 * Form constructor for Bootstrap library selection form
 *
 * @return array
 */
function kalatheme_bootstrap_library_form() {
  $form = array();

  $form['bootstrap'] = array(
    '#type' => 'fieldset',
    '#title' => t('Kalatheme Settings'),
    '#weight' => -99,
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['bootstrap']['bootstrap_library'] = array(
    '#type' => 'select',
    '#title' => t('Select a Bootstrap library.'),
    '#description' => t('Select a Bootstrap library to use with your subtheme. Visit <a href="@bootswatch">Bootswatch</a> for more info. Select "custom" to upload a third party Bootstrap library such as one from <a href="@wrapbootstrap">Wrapbootstrap</a> or built using Bootstrap\'s <a href="@bootcustom">Customization Utility</a>.', array(
      '@bootswatch' => 'http://bootswatch.com/',
      '@wrapbootstrap' => 'http://wrapbootstrap.com/',
      '@bootcustom' => 'http://getbootstrap.com/customize/',
    )),
    '#ajax' => array(
      'callback' => 'kalatheme_ajax_bootstrap_preview_submit',
      'wrapper' => 'kalapreview-image-load-image',
      'method' => 'replace',
      'effect' => 'fade',
      'progress' => array('type' => 'none'),
    ),
  );
  foreach (kalatheme_available_bootstrap_libraries() as $theme) {
    $form['bootstrap']['bootstrap_library']['#options'][$theme] = ucfirst($theme);
  }
  $form['bootstrap']['bootstrap_preview'] = array(
    '#type' => 'markup',
    '#prefix' => "<div id='kalapreview-image-load-image'>",
    '#suffix' => "</div>",
  );
  $form['bootstrap']['bootstrap_upload'] = array(
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

  return $form;
}

/**
 * Form constructor for Bootstrap library selection form
 *
 * @return array
 */
function kalatheme_backend_check_form() {
  $form = array();

  if (kalatheme_backend_check()) {
    $form['backend_check'] = array(
      '#weight' => -100,
      '#prefix' => '<div class="alert alert-success">',
      '#markup' => t("Your webserver is correctly configured to allow for custom Bootstrap libraries and subtheme generation!"),
      '#suffix' => '</div>',
    );
  }
  else {
    $form['backend_check'] = array(
      '#weight' => -100,
      '#prefix' => '<div class="alert alert-danger">',
      '#markup' => t("If you want Kalatheme to be able to use custom Bootstrap libraries or generate subthemes automatically please properly configure your webserver."),
        // @todo add link to docs here
      '#suffix' => '</div>',
    );
  }
  return $form;
}

/**
 * Form constructor for subtheme selection form
 *
 * @return array
 */
function kalatheme_subtheme_form() {
  $form = array();

  if (kalatheme_backend_check()) {
    $form['subtheme'] = array(
      '#type' => 'fieldset',
      '#weight' => -98,
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
    );
    $form['subtheme']['build_subtheme'] = array(
      '#type' => 'checkbox',
      '#title' => t('Build and enable a custom subtheme'),
      '#default_value' => FALSE,
      '#description' => t('Check here to have kalatheme build you a subtheme with the settings on this form.'),
      '#ajax' => array(
        'callback' => 'kalatheme_ajax_submit_submit',
        'wrapper' => 'kala-submit-change-title',
        'progress' => array('type' => 'none'),
      ),
    );
    $form['subtheme']['subtheme_name'] = array(
      '#type' => 'textfield',
      '#title' => t('Name your theme'),
      '#description' => t('No special characters. For example: My Theme, Stuff. If you input nothing it will use your sitename.'),
      '#states' => array(
        'visible' => array(
          ':input[name="build_subtheme"]' => array('checked' => TRUE),
        ),
      ),
    );
    $base_themes = kalatheme_get_base_themes();
    if (count($base_themes) > 1) {
      $form['subtheme']['subtheme_base'] = array(
        '#type' => 'select',
        '#title' => t('Subtheme base'),
        '#description' => t('The base theme to use for your theme.'),
        '#options' => $base_themes,
        '#default_value' => 'kalatheme',
        '#states' => array(
          'visible' => array(
            ':input[name="build_subtheme"]' => array('checked' => TRUE),
          ),
        ),
      );
    }
    else {
      $form['subtheme']['subtheme_base'] = array(
        '#type' => 'hidden',
        '#value' => 'kalatheme',
      );
    }
  }

  return $form;
}

/**
 * Handle custom Bootstrap library validation
 */
function kalatheme_custom_bootstrap_library_validate(&$form, &$form_state) {
  // Validate subtehme name if exists
  $regex = '/^[a-zA-Z][\da-zA-Z _\']*$/';
  if (!empty($form_state['values']['subtheme_name'])) {
    if (!preg_match($regex, $form_state['values']['subtheme_name'])) {
      form_set_error('form', t('Please enter a valid subtheme name.'));
    }
  }
  // Make sure the user has uploaded an archive
  // We handle validation of the archive in the submit handler
  if ($form_state['values']['bootstrap_library'] == 'custom') {
    if (empty($_FILES['files']['name']['bootstrap_upload'])) {
      form_set_error('bootstrap_upload', t('You must provide a custom Bootstrap library.'));
    }
  }
  // Add the default system_form_submit back into the mix if we aren't building
  // a custom subtheme
  if (!$form_state['values']['build_subtheme']) {
    $form['#submit'][] = 'system_theme_settings_submit';
  }
}

/**
 * Handle custom Bootstrap library submission
 */
function kalatheme_custom_bootstrap_library_submit(&$form, &$form_state) {
  // Summon the powerz of update manager
  module_load_include("inc", "update", "update.manager");
  $projects = array();

  // Use a custom bootstrap library
  if ($_FILES['files']['name']['bootstrap_upload']) {
    $projects[] = kalatheme_custom_bootstrap_library_build($form_state['values']['subtheme_name']);
  }

  // Generate subtheme with correct settings
  // put subtheme files into tmp directory
  // put in settings and save settings
  // queue up some args for batching
  if ($form_state['values']['build_subtheme']) {
    // Trim things
    form_state_values_clean($form_state);
    $projects[] = kalatheme_custom_subtheme_build($form_state['values']);
  }

  // Run the batch operation if we have things to move
  if ($projects) {
    // If the owner of the directory we extracted is the same as the
    // owner of our configuration directory (e.g. sites/default) where we're
    // trying to install the code, there's no need to prompt for FTP/SSH
    // credentials. Instead, we instantiate a FileTransferLocal and invoke
    // update_authorize_run_install() directly.
    if (kalatheme_has_write_access()) {
      $filetransfer = new FileTransferLocal(DRUPAL_ROOT);
      call_user_func_array('kalatheme_run_install', array_merge(array($filetransfer), array($projects)));
    }
    // Otherwise, go through the regular workflow to prompt for FTP/SSH
    // credentials and invoke update_authorize_run_install() indirectly with
    // whatever FileTransfer object authorize.php creates for us.
    else {
      system_authorized_init('kalatheme_run_install', drupal_get_path('module', 'update') . '/update.authorize.inc', $projects, t('Update manager'));
      $form_state['redirect'] = system_authorized_get_url();
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
function kalatheme_custom_subtheme_build($theme_settings) {
  $machine_name = kalatheme_get_machine_name($theme_settings['subtheme_name']);
  $directory = kalatheme_extract_directory('theme') . '/' . $machine_name;
  if (kalatheme_write_stuff(kalatheme_custom_subtheme_skeleton($theme_settings), $directory)) {
    // Make sure the Updater registry is loaded.
    drupal_get_updaters();
    try {
      $updater = Updater::factory($directory);
    }
    catch (Exception $e) {
      form_set_error($field, $e->getMessage());
      return;
    }
    $project_real_location = drupal_realpath($directory);
    $arguments = array(
      'project' => $machine_name,
      'updater_name' => get_class($updater),
      'local_url' => $project_real_location,
      'subtheme' => TRUE,
    );

    // Move settings over
    $key = 'theme_' . $machine_name . '_settings';
    // Remove unneeded vars
    unset($theme_settings['var']);
    unset($theme_settings['build_subtheme']);
    unset($theme_settings['bootstrap_upload']);
    unset($theme_settings['subtheme_name']);
    unset($theme_settings['subtheme_base']);

    // If the user uploaded a new logo or favicon, save it to a permanent location
    // and use it in place of the default theme-provided file.
    if ($file = $theme_settings['logo_upload']) {
      unset($theme_settings['logo_upload']);
      $filename = file_unmanaged_copy($file->uri);
      $theme_settings['default_logo'] = 0;
      $theme_settings['logo_path'] = $filename;
      $theme_settings['toggle_logo'] = 1;
    }
    if ($file = $theme_settings['favicon_upload']) {
      unset($theme_settings['favicon_upload']);
      $filename = file_unmanaged_copy($file->uri);
      $theme_settings['default_favicon'] = 0;
      $theme_settings['favicon_path'] = $filename;
      $theme_settings['toggle_favicon'] = 1;
    }

    // If the user entered a path relative to the system files directory for
    // a logo or favicon, store a public:// URI so the theme system can handle it.
    if (!empty($theme_settings['logo_path'])) {
      $theme_settings['logo_path'] = _system_theme_settings_validate_path($theme_settings['logo_path']);
    }
    if (!empty($theme_settings['favicon_path'])) {
      $theme_settings['favicon_path'] = _system_theme_settings_validate_path($theme_settings['favicon_path']);
    }

    if (empty($theme_settings['default_favicon']) && !empty($theme_settings['favicon_path'])) {
      $theme_settings['favicon_mimetype'] = file_get_mimetype($theme_settings['favicon_path']);
    }
    // Save the new stuff
    variable_set($key, $theme_settings);

    // Return the batch
    return $arguments;
  }
  return;
}


/**
 * Return basic subtheme skeleton to be written
 */
function kalatheme_custom_subtheme_skeleton($theme_settings) {
  $subtheme = array();
  $machine_name = kalatheme_get_machine_name($theme_settings['subtheme_name']);
  // Basic Info File
  $output  = "name = " . $theme_settings['subtheme_name'] . " \n";
  $output .= "description = A Kalatheme subtheme powered by Twitter Bootstrap & Science. \n";
  $output .= "base theme = " . $theme_settings['subtheme_base'] . " \n";
  $output .= "core = 7.x \n";
  $output .= "\n";
  $output .= "regions[content] = Content \n";
  $output .= "\n";
  $output .= "stylesheets[all][] = css/main.css \n";
  $output .= "\n";
  $subtheme[DIRECTORY_SEPARATOR . $machine_name . '.info'] = $output;
  // Basic CSS File
  $output  = "/*! \n";
  $output .= " * Kalatheme (http://drupal.org/project/kalatheme) \n";
  $output .= " */ \n";
  $subtheme[DIRECTORY_SEPARATOR . 'css' . DIRECTORY_SEPARATOR . 'main.css'] = $output;
  // Basic Template File
  $output  = "<?php \n";
  $output .= "/** \n";
  $output .= " * @file \n";
  $output .= " * Primary pre/preprocess functions and alterations.\n";
  $output .= " */ \n";
  $subtheme[DIRECTORY_SEPARATOR . 'template.php'] = $output;

  return $subtheme;
}

/**
 * Handle custom Bootstrap library submission
 */
function kalatheme_custom_bootstrap_library_build($subtheme_name) {
  // Save the file to tmp
  $validators = array('file_validate_extensions' => array(archiver_get_extensions()));
  $field = 'bootstrap_upload';
  if (!($finfo = file_save_upload($field, $validators, NULL, FILE_EXISTS_REPLACE))) {
    // Failed to upload the file. file_save_upload() calls form_set_error() on
    // failure.
    return;
  }
  $local_cache = $finfo->uri;

  // extract file to tmp
  $project = 'kalatheme_bootstrap';
  if ($subtheme_name) {
    $project = kalatheme_get_machine_name($subtheme_name . '_bootstrap');
  }
  $directory = kalatheme_extract_directory('library') . '/' . $project;
  try {
    $archive = update_manager_archive_extract($local_cache, $directory);
  }
  catch (Exception $e) {
    form_set_error($field, $e->getMessage());
    return;
  }

  // validate archive
  $files = $archive->listContents();
  if (!$files) {
    form_set_error($field, t('Provided archive contains no files.'));
    return;
  }
  foreach ($files as $file) {
    if (stripos($file, 'bootstrap.min.css') !== FALSE || stripos($file, 'bootstrap.css') !== FALSE) {
      $check_file = $file;
      break;
    }
  }
  if (!file_exists($directory . '/' . $check_file)) {
    form_set_error($field, t('Provided archive contains no files.'));
    return;
  }

  // Loop of projects
  // set projects that need to be moved
  // Do this per project
  // Make sure the Updater registry is loaded.
  require_once dirname(__FILE__) . '/../kalatheme.updater.inc';
  drupal_get_updaters();
  try {
    $updater = Updater::factory($directory);
  }
  catch (Exception $e) {
    form_set_error($field, $e->getMessage());
    return;
  }
  $project_real_location = drupal_realpath($directory);
  $arguments = array(
    'project' => $project,
    'updater_name' => get_class($updater),
    'local_url' => $project_real_location,
    'subtheme' => FALSE,
  );

  return $arguments;
}


/**
 * The batch builder and processor for moving files to drupal
 *
 * taken from update_authorize_run_update
 * builds a batch and process it for installing modules from the templocation
 */
function kalatheme_run_install($filetransfer, $projects) {
  $operations = array();
  foreach ($projects as $project_info) {
    $operations[] = array(
      'kalatheme_authorize_batch_copy_project',
      array(
        $project_info['project'],
        $project_info['updater_name'],
        $project_info['local_url'],
        $project_info['subtheme'],
        $filetransfer,
      ),
    );
  }

  // @todo Instantiate our Updater to set the human-readable title?
  $batch = array(
    'title' => t('Installing Kalatheme'),
    'init_message' => t('Preparing to Kalatheme'),
    'operations' => $operations,
    // @todo Use a different finished callback for different messages?
    'finished' => 'kalatheme_authorize_install_batch_finished',
    'file' => drupal_get_path('theme', 'kalatheme') . '/kalatheme.authorize.inc',
  );
  batch_set($batch);
}

/**
 * List of all themes available from bootswatch; only scalar values can be
 * constants.
 *
 * @return array
 */
function kalatheme_available_bootstrap_libraries() {
  $libs =  array(
    'default',
    'amelia',
    'cerulean',
    'cosmo',
    'cyborg',
    'flatly',
    'journal',
    'lumen',
    'readable',
    'simplex',
    'slate',
    'spacelab',
    'superhero',
    'united',
    'yeti',
    'none'
  );
  if (kalatheme_backend_check()){
    array_push($libs, 'custom');
  }
  return $libs;
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
function kalatheme_ajax_bootstrap_preview_submit($form, &$form_state) {
  $theme = $form_state['values']['bootstrap_library'];
  if ($theme == 'default' || $theme == 'custom' || $theme == 'none') {
    $form['bootstrap']['bootstrap_preview']['#markup'] = t('');
  }
  else {
    // @todo: Do this in a way that doesn't suck and maybe incorporate some
    // styles as seen on bootswatch?
    $form['bootstrap']['bootstrap_preview']['#markup'] = t('<img src="@preview">', array('@preview' => 'http://bootswatch.com/' . $theme . '/thumbnail.png'));
  }
  return $form['bootstrap']['bootstrap_preview'];
}

/**
 * Ajax callback that returns HTML to the submit button so that the user
 * has clearer UX actions based on form selection
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
function kalatheme_ajax_submit_submit($form, &$form_state) {
  $subtheme = $form_state['values']['build_subtheme'];
  $form['actions']['submit']['#value'] = ($subtheme) ? t('Build and enable subtheme') : t('Save configuration');
  return $form['actions']['submit'];
}


/**
 * We want to check if the webserver can write our stuff to the right places.
 * If it can't then we are going to tell the user what's up.
 *
 * @return
 *   True if all good, message if no good
 */
function kalatheme_backend_check() {
  // Verify FTP support
  $ftp_installed = extension_loaded('ftp');
  // Verify SSH support
  $ssh_installed = extension_loaded('ssh2');
  // Verify web server write permissions
  $install_permissions = kalatheme_has_write_access();
  // Verify update module is enabled
  $updates_module = module_exists('update');

  return (($ftp_installed || $ssh_installed || $install_permissions) && $updates_module);
}

/**
 * Check whether Kalatheme has write access to libraries and modules directories.
 *
 * @return boolean
 */
function kalatheme_has_write_access() {
  return is_writable(drupal_realpath(kalatheme_lib_dir())) && is_writable(drupal_realpath('sites/all/modules'));
}

/**
 * Return the libraries dir to check
 */
function kalatheme_lib_dir() {
  if (!is_dir('sites/all/libraries')) {
    mkdir(DRUPAL_ROOT . '/sites/all/libraries');
  }
  return is_dir('sites/all/libraries') ? 'sites/all/libraries' : 'sites/all';
}

/**
 * Wrapper for _update_manager_extract_directory()
 *
 * Since libraries and themes can live in the same location, we need to
 * namespace the types so they don't collide like the colorbox module.
 */
function kalatheme_extract_directory($type = '') {
  $directory = _update_manager_extract_directory();
  if ($type) {
    $directory .= '/' . $type;
    if (!file_exists($directory)) {
      mkdir($directory);
    }
  }
  return $directory;
}

/**
 * Returns a list of Kalatheme base themes.
 */
function kalatheme_get_base_themes() {
  $themes = list_themes();

  $base_themes = array('kalatheme' => 'Kalatheme');
  if (isset($themes['kalatheme']->sub_themes)) {
    $base_themes += $themes['kalatheme']->sub_themes;
  }

  return $base_themes;
}

/**
 * Translate human readable to machine name
 */
function kalatheme_get_machine_name($name) {
  $machine_name = strtolower($name);
  $machine_name = preg_replace('@[^a-z0-9_]+@', '_', $machine_name);

  return $machine_name;
}

/**
 * Translate a subtheme array into writable files.
 *
 * @param $subtheme
 *   Array of stuff to write
 *
 * @return
 *   TRUE is for success, FALSE is for fail
 *
 */
function kalatheme_write_stuff($subtheme, $directory) {
  foreach ($subtheme as $key => $value) {
    $filename = $directory . $key;
    $dir = substr($filename, 0, strrpos($filename, '/'));
    if (is_dir($dir) || mkdir($dir, 0777, TRUE)) {
      if (!file_put_contents($filename, $value)) {
        return FALSE;
      }
    }
    else {
      return FALSE;
    }
  }
  return TRUE;
}
