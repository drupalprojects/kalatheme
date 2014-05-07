<?php

/**
 * @file
 * Kalatheme's theme functions for icons
 */

/**
 * Theme function for icon fonts.
 */
function kalatheme_font_icon($variables){
  $attr = isset($variables['attributes']) ? $variables['attributes'] : array();
  $attr += array('aria-hidden' => 'true' );
  $attr['class'] = array();
  $icon_font_prefix = 'glyicon-';

  if( theme_get_setting('fontawesome') ){
    $icon_font_prefix = 'fa-';
    $attr['class'][] = 'fa';
  }
  else{
    $attr['class'][] = 'glyphicon';
  }

  foreach($variables['keys'] as $class){
    $attr['class'][] = $icon_font_prefix . $class;
  }
  $stub = '<span '. drupal_attributes($attr) . '></span>';
  if(isset($variables['alt_text'])){
    $stub .= '<span class="element-invisible">'.$variables['alt_text'].'</span>';
  }
  return $stub;
}



/**
 * Implements hook_icon_providers().
 */
function kalatheme_icon_providers() {
  $providers['font_awesome'] = array(
    'title' => t('Font Awesome'),
    'url' => 'http://fontawesome.io',
    'default bundle' => array(
      'render' => 'html_tag',
      'settings' => kalatheme_icon_default_settings(),
    ),
  );

  $providers['bootstrap_gyphicons'] = array(
    'title' => t('Bootstrap Gypicons'),
    'url' => 'http://getbootstrap.com/components/#glyphicons',
    'default bundle' => array(
      'render' => 'html_tag',
      'settings' => kalatheme_icon_default_settings(),
    ),
  );
  return $providers;
}

/**
 * Provides the default settings for this
 */
function kalatheme_icon_default_settings(){
  return array(
    'tag' => 'span'
  );
}


/**
 * Implements hook_icon_bundle_configure().
 */
function kalatheme_icon_bundle_configure(&$settings, &$form_state, &$complete_form) {
  $bundle = $form_state['bundle'];
  if ($bundle['provider'] === 'font_awesome' || 'bootstrap_gyphicons') {
    $settings['tag'] = array(
      '#type' => 'select',
      '#title' => t('HTML Markup'),
      '#description' => t('Choose the HTML markup tag that Font Awesome icons should be created with.
        Typically, this is an %span tag, however it can be changed to suite the theme requirements.', array('%span' => '<span/>')),
      '#options' => drupal_map_assoc(array('i', 'span', 'div')),
      '#default_value' => isset($bundle['settings']['tag']) ? $bundle['settings']['tag'] : 'span',
    );
  }
}

/**
 *
 */
function kalatheme_preprocess_icon_html_tag(&$variables) {
  $bundle = &$variables['bundle'];
  $icon = &$variables['icon'];
  $icon_font_prefix = '';
  if( $bundle === 'font_awesome' ){
    $icon_font_prefix = 'fa-';
  }
  elseif($bundle === 'bootstrap_glyphicon' ){
    $icon_font_prefix = 'glyicon-';
  }
}

/**
 * @desc return HTML tag emement with fontawesome icon classed
 * @param <type> $variables
 * @return <type>
 */
function theme_icon_html_tag($variables) {
  $output = '';
  $bundle = $variables['bundle'];
  $icon = $variables['icon'];
  $output = theme('html_tag', array(
    'element' => array(
      '#tag' => isset($bundle['settings']['tag']) ? $bundle['settings']['tag'] : 'span',
      '#attributes' => $variables['attributes'],
      '#value' =>' ',
    ),
  ));
  return $output;
}

/**
 * Implements hook_preprocess_icon().
 */
function fontawesome_preprocess_icon(&$variables) {
  if (!empty($variables['bundle']['provider'])){
    $classes = array();
    $classes[] = drupal_html_class($icon);
    switch($variables['bundle']['provider']){
      case 'font_awesome':
        $classes[] = 'fa';
        break;
      case 'bootstrap_glyhicons':
        $classes[] = 'glyphicon';
        break;
      default:
    }

    $variables['attributes']['class'] += $classes;
  }
}
/**
 * Implements hook_icon_bundles().
 */
function kalatheme_icon_bundles() {
  $bundles['font_awesome'] = $bundles['fontawesome'] = array(
    'render' => 'html_tag',
    'provider' => 'font_awesome',
    'title' => t('FontAwsome: Icons'),
    'icons' => array(
      'rub' => 'rub',
      'ruble' => 'ruble',
      'rouble' => 'rouble',
      'pagelines' => 'pagelines',
      'stack-exchange' => 'stack-exchange',
      'arrow-circle-o-right' => 'arrow-circle-o-right',
      'arrow-circle-o-left' => 'arrow-circle-o-left',
      'caret-square-o-left' => 'caret-square-o-left',
      'toggle-left' => 'toggle-left',
      'dot-circle-o' => 'dot-circle-o',
      'wheelchair' => 'wheelchair',
      'vimeo-square' => 'vimeo-square',
      'try' => 'try',
      'turkish-lira' => 'turkish-lira',
      'plus-square-o' => 'plus-square-o',
      'adjust' => 'adjust',
      'anchor' => 'anchor',
      'archive' => 'archive',
      'arrows' => 'arrows',
      'arrows-h' => 'arrows-h',
      'arrows-v' => 'arrows-v',
      'asterisk' => 'asterisk',
      'ban' => 'ban',
      'bar-chart-o' => 'bar-chart-o',
      'barcode' => 'barcode',
      'bars' => 'bars',
      'beer' => 'beer',
      'bell' => 'bell',
      'bell-o' => 'bell-o',
      'bolt' => 'bolt',
      'book' => 'book',
      'bookmark' => 'bookmark',
      'bookmark-o' => 'bookmark-o',
      'briefcase' => 'briefcase',
      'bug' => 'bug',
      'building-o' => 'building-o',
      'bullhorn' => 'bullhorn',
      'bullseye' => 'bullseye',
      'calendar' => 'calendar',
      'calendar-o' => 'calendar-o',
      'camera' => 'camera',
      'camera-retro' => 'camera-retro',
      'caret-square-o-down' => 'caret-square-o-down',
      'caret-square-o-left' => 'caret-square-o-left',
      'caret-square-o-right' => 'caret-square-o-right',
      'caret-square-o-up' => 'caret-square-o-up',
      'certificate' => 'certificate',
      'check' => 'check',
      'check-circle' => 'check-circle',
      'check-circle-o' => 'check-circle-o',
      'check-square' => 'check-square',
      'check-square-o' => 'check-square-o',
      'circle' => 'circle',
      'circle-o' => 'circle-o',
      'clock-o' => 'clock-o',
      'cloud' => 'cloud',
      'cloud-download' => 'cloud-download',
      'cloud-upload' => 'cloud-upload',
      'code' => 'code',
      'code-fork' => 'code-fork',
      'coffee' => 'coffee',
      'cog' => 'cog',
      'cogs' => 'cogs',
      'comment' => 'comment',
      'comment-o' => 'comment-o',
      'comments' => 'comments',
      'comments-o' => 'comments-o',
      'compass' => 'compass',
      'credit-card' => 'credit-card',
      'crop' => 'crop',
      'crosshairs' => 'crosshairs',
      'cutlery' => 'cutlery',
      'dashboard' => 'dashboard',
      'desktop' => 'desktop',
      'dot-circle-o' => 'dot-circle-o',
      'download' => 'download',
      'edit' => 'edit',
      'ellipsis-h' => 'ellipsis-h',
      'ellipsis-v' => 'ellipsis-v',
      'envelope' => 'envelope',
      'envelope-o' => 'envelope-o',
      'eraser' => 'eraser',
      'exchange' => 'exchange',
      'exclamation' => 'exclamation',
      'exclamation-circle' => 'exclamation-circle',
      'exclamation-triangle' => 'exclamation-triangle',
      'external-link' => 'external-link',
      'external-link-square' => 'external-link-square',
      'eye' => 'eye',
      'eye-slash' => 'eye-slash',
      'female' => 'female',
      'fighter-jet' => 'fighter-jet',
      'film' => 'film',
      'filter' => 'filter',
      'fire' => 'fire',
      'fire-extinguisher' => 'fire-extinguisher',
      'flag' => 'flag',
      'flag-checkered' => 'flag-checkered',
      'flag-o' => 'flag-o',
      'flash' => 'flash',
      'flask' => 'flask',
      'folder' => 'folder',
      'folder-o' => 'folder-o',
      'folder-open' => 'folder-open',
      'folder-open-o' => 'folder-open-o',
      'frown-o' => 'frown-o',
      'gamepad' => 'gamepad',
      'gavel' => 'gavel',
      'gear' => 'gear',
      'gears' => 'gears',
      'gift' => 'gift',
      'glass' => 'glass',
      'globe' => 'globe',
      'group' => 'group',
      'hdd-o' => 'hdd-o',
      'headphones' => 'headphones',
      'heart' => 'heart',
      'heart-o' => 'heart-o',
      'home' => 'home',
      'inbox' => 'inbox',
      'info' => 'info',
      'info-circle' => 'info-circle',
      'key' => 'key',
      'keyboard-o' => 'keyboard-o',
      'laptop' => 'laptop',
      'leaf' => 'leaf',
      'legal' => 'legal',
      'lemon-o' => 'lemon-o',
      'level-down' => 'level-down',
      'level-up' => 'level-up',
      'lightbulb-o' => 'lightbulb-o',
      'location-arrow' => 'location-arrow',
      'lock' => 'lock',
      'magic' => 'magic',
      'magnet' => 'magnet',
      'mail-forward' => 'mail-forward',
      'mail-reply' => 'mail-reply',
      'mail-reply-all' => 'mail-reply-all',
      'male' => 'male',
      'map-marker' => 'map-marker',
      'meh-o' => 'meh-o',
      'microphone' => 'microphone',
      'microphone-slash' => 'microphone-slash',
      'minus' => 'minus',
      'minus-circle' => 'minus-circle',
      'minus-square' => 'minus-square',
      'minus-square-o' => 'minus-square-o',
      'mobile' => 'mobile',
      'mobile-phone' => 'mobile-phone',
      'money' => 'money',
      'moon-o' => 'moon-o',
      'music' => 'music',
      'pencil' => 'pencil',
      'pencil-square' => 'pencil-square',
      'pencil-square-o' => 'pencil-square-o',
      'phone' => 'phone',
      'phone-square' => 'phone-square',
      'picture-o' => 'picture-o',
      'plane' => 'plane',
      'plus' => 'plus',
      'plus-circle' => 'plus-circle',
      'plus-square' => 'plus-square',
      'plus-square-o' => 'plus-square-o',
      'power-off' => 'power-off',
      'print' => 'print',
      'puzzle-piece' => 'puzzle-piece',
      'qrcode' => 'qrcode',
      'question' => 'question',
      'question-circle' => 'question-circle',
      'quote-left' => 'quote-left',
      'quote-right' => 'quote-right',
      'random' => 'random',
      'refresh' => 'refresh',
      'reply' => 'reply',
      'reply-all' => 'reply-all',
      'retweet' => 'retweet',
      'road' => 'road',
      'rocket' => 'rocket',
      'rss' => 'rss',
      'rss-square' => 'rss-square',
      'search' => 'search',
      'search-minus' => 'search-minus',
      'search-plus' => 'search-plus',
      'share' => 'share',
      'share-square' => 'share-square',
      'share-square-o' => 'share-square-o',
      'shield' => 'shield',
      'shopping-cart' => 'shopping-cart',
      'sign-in' => 'sign-in',
      'sign-out' => 'sign-out',
      'signal' => 'signal',
      'sitemap' => 'sitemap',
      'smile-o' => 'smile-o',
      'sort' => 'sort',
      'sort-alpha-asc' => 'sort-alpha-asc',
      'sort-alpha-desc' => 'sort-alpha-desc',
      'sort-amount-asc' => 'sort-amount-asc',
      'sort-amount-desc' => 'sort-amount-desc',
      'sort-asc' => 'sort-asc',
      'sort-desc' => 'sort-desc',
      'sort-down' => 'sort-down',
      'sort-numeric-asc' => 'sort-numeric-asc',
      'sort-numeric-desc' => 'sort-numeric-desc',
      'sort-up' => 'sort-up',
      'spinner' => 'spinner',
      'square' => 'square',
      'square-o' => 'square-o',
      'star' => 'star',
      'star-half' => 'star-half',
      'star-half-empty' => 'star-half-empty',
      'star-half-full' => 'star-half-full',
      'star-half-o' => 'star-half-o',
      'star-o' => 'star-o',
      'subscript' => 'subscript',
      'suitcase' => 'suitcase',
      'sun-o' => 'sun-o',
      'superscript' => 'superscript',
      'tablet' => 'tablet',
      'tachometer' => 'tachometer',
      'tag' => 'tag',
      'tags' => 'tags',
      'tasks' => 'tasks',
      'terminal' => 'terminal',
      'thumb-tack' => 'thumb-tack',
      'thumbs-down' => 'thumbs-down',
      'thumbs-o-down' => 'thumbs-o-down',
      'thumbs-o-up' => 'thumbs-o-up',
      'thumbs-up' => 'thumbs-up',
      'ticket' => 'ticket',
      'times' => 'times',
      'times-circle' => 'times-circle',
      'times-circle-o' => 'times-circle-o',
      'tint' => 'tint',
      'toggle-down' => 'toggle-down',
      'toggle-left' => 'toggle-left',
      'toggle-right' => 'toggle-right',
      'toggle-up' => 'toggle-up',
      'trash-o' => 'trash-o',
      'trophy' => 'trophy',
      'truck' => 'truck',
      'umbrella' => 'umbrella',
      'unlock' => 'unlock',
      'unlock-alt' => 'unlock-alt',
      'unsorted' => 'unsorted',
      'upload' => 'upload',
      'user' => 'user',
      'users' => 'users',
      'video-camera' => 'video-camera',
      'volume-down' => 'volume-down',
      'volume-off' => 'volume-off',
      'volume-up' => 'volume-up',
      'warning' => 'warning',
      'wheelchair' => 'wheelchair',
      'wrench' => 'wrench',
      'check-square' => 'check-square',
      'check-square-o' => 'check-square-o',
      'circle' => 'circle',
      'circle-o' => 'circle-o',
      'dot-circle-o' => 'dot-circle-o',
      'minus-square' => 'minus-square',
      'minus-square-o' => 'minus-square-o',
      'plus-square' => 'plus-square',
      'plus-square-o' => 'plus-square-o',
      'square' => 'square',
      'square-o' => 'square-o',
      'bitcoin' => 'bitcoin',
      'btc' => 'btc',
      'cny' => 'cny',
      'dollar' => 'dollar',
      'eur' => 'eur',
      'euro' => 'euro',
      'gbp' => 'gbp',
      'inr' => 'inr',
      'jpy' => 'jpy',
      'krw' => 'krw',
      'money' => 'money',
      'rmb' => 'rmb',
      'rouble' => 'rouble',
      'rub' => 'rub',
      'ruble' => 'ruble',
      'rupee' => 'rupee',
      'try' => 'try',
      'turkish-lira' => 'turkish-lira',
      'usd' => 'usd',
      'won' => 'won',
      'yen' => 'yen',
      'align-center' => 'align-center',
      'align-justify' => 'align-justify',
      'align-left' => 'align-left',
      'align-right' => 'align-right',
      'bold' => 'bold',
      'chain' => 'chain',
      'chain-broken' => 'chain-broken',
      'clipboard' => 'clipboard',
      'columns' => 'columns',
      'copy' => 'copy',
      'cut' => 'cut',
      'dedent' => 'dedent',
      'eraser' => 'eraser',
      'file' => 'file',
      'file-o' => 'file-o',
      'file-text' => 'file-text',
      'file-text-o' => 'file-text-o',
      'files-o' => 'files-o',
      'floppy-o' => 'floppy-o',
      'font' => 'font',
      'indent' => 'indent',
      'italic' => 'italic',
      'link' => 'link',
      'list' => 'list',
      'list-alt' => 'list-alt',
      'list-ol' => 'list-ol',
      'list-ul' => 'list-ul',
      'outdent' => 'outdent',
      'paperclip' => 'paperclip',
      'paste' => 'paste',
      'repeat' => 'repeat',
      'rotate-left' => 'rotate-left',
      'rotate-right' => 'rotate-right',
      'save' => 'save',
      'scissors' => 'scissors',
      'strikethrough' => 'strikethrough',
      'table' => 'table',
      'text-height' => 'text-height',
      'text-width' => 'text-width',
      'th' => 'th',
      'th-large' => 'th-large',
      'th-list' => 'th-list',
      'underline' => 'underline',
      'undo' => 'undo',
      'unlink' => 'unlink',
      'angle-double-down' => 'angle-double-down',
      'angle-double-left' => 'angle-double-left',
      'angle-double-right' => 'angle-double-right',
      'angle-double-up' => 'angle-double-up',
      'angle-down' => 'angle-down',
      'angle-left' => 'angle-left',
      'angle-right' => 'angle-right',
      'angle-up' => 'angle-up',
      'arrow-circle-down' => 'arrow-circle-down',
      'arrow-circle-left' => 'arrow-circle-left',
      'arrow-circle-o-down' => 'arrow-circle-o-down',
      'arrow-circle-o-left' => 'arrow-circle-o-left',
      'arrow-circle-o-right' => 'arrow-circle-o-right',
      'arrow-circle-o-up' => 'arrow-circle-o-up',
      'arrow-circle-right' => 'arrow-circle-right',
      'arrow-circle-up' => 'arrow-circle-up',
      'arrow-down' => 'arrow-down',
      'arrow-left' => 'arrow-left',
      'arrow-right' => 'arrow-right',
      'arrow-up' => 'arrow-up',
      'arrows' => 'arrows',
      'arrows-alt' => 'arrows-alt',
      'arrows-h' => 'arrows-h',
      'arrows-v' => 'arrows-v',
      'caret-down' => 'caret-down',
      'caret-left' => 'caret-left',
      'caret-right' => 'caret-right',
      'caret-square-o-down' => 'caret-square-o-down',
      'caret-square-o-left' => 'caret-square-o-left',
      'caret-square-o-right' => 'caret-square-o-right',
      'caret-square-o-up' => 'caret-square-o-up',
      'caret-up' => 'caret-up',
      'chevron-circle-down' => 'chevron-circle-down',
      'chevron-circle-left' => 'chevron-circle-left',
      'chevron-circle-right' => 'chevron-circle-right',
      'chevron-circle-up' => 'chevron-circle-up',
      'chevron-down' => 'chevron-down',
      'chevron-left' => 'chevron-left',
      'chevron-right' => 'chevron-right',
      'chevron-up' => 'chevron-up',
      'hand-o-down' => 'hand-o-down',
      'hand-o-left' => 'hand-o-left',
      'hand-o-right' => 'hand-o-right',
      'hand-o-up' => 'hand-o-up',
      'long-arrow-down' => 'long-arrow-down',
      'long-arrow-left' => 'long-arrow-left',
      'long-arrow-right' => 'long-arrow-right',
      'long-arrow-up' => 'long-arrow-up',
      'toggle-down' => 'toggle-down',
      'toggle-left' => 'toggle-left',
      'toggle-right' => 'toggle-right',
      'toggle-up' => 'toggle-up',
      'arrows-alt' => 'arrows-alt',
      'backward' => 'backward',
      'compress' => 'compress',
      'eject' => 'eject',
      'expand' => 'expand',
      'fast-backward' => 'fast-backward',
      'fast-forward' => 'fast-forward',
      'forward' => 'forward',
      'pause' => 'pause',
      'play' => 'play',
      'play-circle' => 'play-circle',
      'play-circle-o' => 'play-circle-o',
      'step-backward' => 'step-backward',
      'step-forward' => 'step-forward',
      'stop' => 'stop',
      'youtube-play' => 'youtube-play',
      'adn' => 'adn',
      'android' => 'android',
      'apple' => 'apple',
      'bitbucket' => 'bitbucket',
      'bitbucket-square' => 'bitbucket-square',
      'bitcoin' => 'bitcoin',
      'btc' => 'btc',
      'css3' => 'css3',
      'dribbble' => 'dribbble',
      'dropbox' => 'dropbox',
      'facebook' => 'facebook',
      'facebook-square' => 'facebook-square',
      'flickr' => 'flickr',
      'foursquare' => 'foursquare',
      'github' => 'github',
      'github-alt' => 'github-alt',
      'github-square' => 'github-square',
      'gittip' => 'gittip',
      'google-plus' => 'google-plus',
      'google-plus-square' => 'google-plus-square',
      'html5' => 'html5',
      'instagram' => 'instagram',
      'linkedin' => 'linkedin',
      'linkedin-square' => 'linkedin-square',
      'linux' => 'linux',
      'maxcdn' => 'maxcdn',
      'pagelines' => 'pagelines',
      'pinterest' => 'pinterest',
      'pinterest-square' => 'pinterest-square',
      'renren' => 'renren',
      'skype' => 'skype',
      'stack-exchange' => 'stack-exchange',
      'stack-overflow' => 'stack-overflow',
      'trello' => 'trello',
      'tumblr' => 'tumblr',
      'tumblr-square' => 'tumblr-square',
      'twitter' => 'twitter',
      'twitter-square' => 'twitter-square',
      'vimeo-square' => 'vimeo-square',
      'vk' => 'vk',
      'weibo' => 'weibo',
      'windows' => 'windows',
      'xing' => 'xing',
      'xing-square' => 'xing-square',
      'youtube' => 'youtube',
      'youtube-play' => 'youtube-play',
      'youtube-square' => 'youtube-square',
      'ambulance' => 'ambulance',
      'h-square' => 'h-square',
      'hospital-o' => 'hospital-o',
      'medkit' => 'medkit',
      'plus-square' => 'plus-square',
      'stethoscope' => 'stethoscope',
      'user-md' => 'user-md',
      'wheelchair' => 'wheelchair',
    ),
  );
  return $bundles;
}
