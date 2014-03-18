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
      '#default_value' => theme_get_setting('build_subtheme'),
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
    $base_themes = _kalatheme_get_base_themes();
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
  // Validate subtehme name ig exists
  $regex = '/^[a-zA-Z][\da-zA-Z _\']*$/';
  if (!preg_match($regex, $form_state['values']['subtheme_name'])) {
    form_set_error('subtheme_name', t('Please enter a valid machine name.'));
  }
  // Make sure the user has uploaded an archive
  // We handle validation of the archive in the submit handler
  if ($form_state['values']['bootstrap_library'] == 'custom') {
    if (empty($_FILES['files']['name']['bootstrap_upload'])) {
      form_set_error('bootstrap_upload', t('You must provide a custom Bootstrap library.'));
    }
  }
}

/**
 * Handle custom Bootstrap library submission
 */
function kalatheme_custom_bootstrap_library_submit(&$form, &$form_state) {
  // Save the uploaded file
  if ($_FILES['files']['name']['bootstrap_upload']) {
    // Summon the power of update manager
    module_load_include("inc", "update", "update.manager");
    $validators = array('file_validate_extensions' => array(archiver_get_extensions()));
    $field = 'bootstrap_upload';
    if (!($finfo = file_save_upload($field, $validators, NULL, FILE_EXISTS_REPLACE))) {
      // Failed to upload the file. file_save_upload() calls form_set_error() on
      // failure.
      return;
    }
    $local_cache = $finfo->uri;

    $project = 'kalatheme_bootstrap';
    $directory = kalatheme_extract_directory('library') . '/' . $project;
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
    );


    // If the owner of the directory we extracted is the same as the
    // owner of our configuration directory (e.g. sites/default) where we're
    // trying to install the code, there's no need to prompt for FTP/SSH
    // credentials. Instead, we instantiate a FileTransferLocal and invoke
    // update_authorize_run_install() directly.
    if (kalatheme_has_write_access()) {
      $filetransfer = new FileTransferLocal(DRUPAL_ROOT);
      call_user_func_array('kalatheme_run_install', array_merge(array($filetransfer), $arguments));
    }
    // Otherwise, go through the regular workflow to prompt for FTP/SSH
    // credentials and invoke update_authorize_run_install() indirectly with
    // whatever FileTransfer object authorize.php creates for us.
    else {
      system_authorized_init('kalatheme_run_install', drupal_get_path('module', 'update') . '/update.authorize.inc', $arguments, t('Update manager'));
      $form_state['redirect'] = system_authorized_get_url();
    }
  }
}


/**
 * The batch builder and processor for moving files to drupal
 *
 * taken from update_authorize_run_update
 * builds a batch and process it for installing modules from the templocation
 */
function kalatheme_run_install($filetransfer, $project, $updater_name, $local_url) {
  $operations[] = array(
    'kalatheme_authorize_batch_copy_project',
    array(
      $project,
      $updater_name,
      $local_url,
      $filetransfer,
    ),
  );

  // @todo Instantiate our Updater to set the human-readable title?
  $batch = array(
    'title' => t('Installing %project', array('%project' => $project)),
    'init_message' => t('Preparing to install'),
    'operations' => $operations,
    // @todo Use a different finished callback for different messages?
    'finished' => 'update_authorize_install_batch_finished',
    'file' => drupal_get_path('theme', 'kalatheme') . '/includes/kalatheme.authorize.inc',
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
function _kalatheme_get_base_themes() {
  $themes = list_themes();

  $base_themes = array('kalatheme' => 'Kalatheme');
  if (isset($themes['kalatheme']->sub_themes)) {
    $base_themes += $themes['kalatheme']->sub_themes;
  }

  return $base_themes;
}
