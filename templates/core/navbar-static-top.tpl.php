<?php
/**
 * @file
 * Kalatheme's static header and navigation bar template file
 *
 *
 */
?>
<header class="navbar navbar-default <?php if ($hide_site_name && $hide_site_slogan && !$logo && !$main_menu && !$secondary_menu) { print ' element-invisible'; } ?>">
  <div class="container">
    <div class="navbar-header">
      <button class="navbar-toggle" type="button" data-toggle="collapse" data-target=".navbar-collapse">
        <span class="element-invisible">Toggle navigation</span>
        <span class="icon-bar" aria-hidden="true"></span>
        <span class="icon-bar" aria-hidden="true"></span>
        <span class="icon-bar" aria-hidden="true"></span>
      </button>
      <?php if ($logo): ?>
        <div class='brand navbar-brand'>
          <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home" id="logo">
            <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
          </a>
        </div>
      <?php endif; ?>

      <?php if ($site_name || $site_slogan): ?>
        <div id="site-name-slogan" class="brand navbar-brand <?php if ($hide_site_name && $hide_site_slogan) { print ' element-invisible'; } ?>">

          <?php if ($site_name): ?>
            <h1 id="site-name"<?php if ($hide_site_name) { print ' class="element-invisible"'; } ?>>
              <strong>
                <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><span><?php print $site_name; ?></span></a>
              </strong>
            </h1>
          <?php endif; ?>

          <?php if ($site_slogan): ?>
            <div id="site-slogan" <?php if ($hide_site_slogan) { print 'class="element-invisible"'; } ?>>
              <?php print $site_slogan; ?>
            </div>
          <?php endif; ?>

        </div> <!-- /#name-and-slogan -->
      <?php endif; ?>
    </div><!-- /.navbar-header -->

    <nav class="collapse navbar-collapse <?php if (!$main_menu && !$secondary_menu) { print 'element-invisible'; } ?>" role="navigation">
      <?php
        $pri_attributes = array(
          'class' => array(
            'nav',
            'navbar-nav',
            'links',
            'clearfix',
          ),
        );
        if (!$main_menu) {
          $pri_attributes['class'][] = 'element-invisible';
        }
      ?>
      <?php print theme('links__system_main_menu', array(
        'links' => $main_menu_expanded,
        'attributes' => $pri_attributes,
        'heading' => array(
          'text' => t('Main menu'),
          'level' => 'h3',
          'class' => array('element-invisible'),
        ),
      )); ?>

      <?php
        $sec_attributes = array(
          'id' => 'secondary-menu-links',
          'class' => array('nav', 'navbar-nav', 'secondary-links'),
        );
        if (!$secondary_menu) {
          $sec_attributes['class'][] = 'element-invisible';
        }
      ?>

      <?php print theme('links__system_secondary_menu', array(
        'links' => $secondary_menu,
        'attributes' => $sec_attributes,
        'heading' => array(
          'text' => t('Secondary menu'),
          'level' => 'h3',
          'class' => array('element-invisible'),
        ),
      )); ?>
    </nav>

  </div>
</header>
