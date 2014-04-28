<?php
/**
 * @file
 * Kalatheme's seperated header and navigation bar template file
 *
 *
 */
?>


<header role="banner">
 <div class="container">
  <div class="row">
    <div class="col-sm-6">
    <?php if ($logo): ?>
      <div class='brand pull-left'>
        <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home" id="logo">
          <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
        </a>
      </div>
    <?php endif; ?>




        <?php if ($site_name): ?>
          <h2 id="site-name"<?php if ($hide_site_name) { print ' class="element-invisible"'; } ?>>
            <strong>
              <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><span><?php print $site_name; ?></span></a>
            </strong>
          </h2>
        <?php endif; ?>
    </div><!-- /.col-sm-6 -->

    <div class="col-sm-6">
      <?php if ($site_slogan): ?>
        <div id="site-slogan" <?php if ($hide_site_slogan) { print 'class="element-invisible"'; } ?>>
          <p class="lead text-right">
            <?php print $site_slogan; ?>
          </p>
        </div>
      <?php endif; ?>
    </div><!-- /.col-sm-6 -->

  </div><!--/.row-->

 </div><!--/.container-->

</header>
<div class="container">
  <nav class="navbar navbar-default  role="navigation"<?php if ($main_menu && !$secondary_menu) { print ' element-invisible'; } ?>">
    <div class="container-fluid">
      <div class="navbar-header">
        <button class="navbar-toggle" type="button" data-toggle="collapse" data-target=".navbar-collapse">
          <span class="element-invisible">Toggle navigation</span>
          <span class="icon-bar" aria-hidden="true"></span>
          <span class="icon-bar" aria-hidden="true"></span>
          <span class="icon-bar" aria-hidden="true"></span>
        </button>
        <?php if ($site_name): ?>
          <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home" class="navbar-brand"><?php print $site_name; ?></a>
        <?php endif; ?>
      </div><!-- /.navbar-header -->

      <div class="collapse navbar-collapse <?php if (!$main_menu && !$secondary_menu) { print 'element-invisible'; } ?>">
        <?php
          $pri_attributes = array(
            'class' => array(
              'nav',
              'navbar-nav',
              'links'
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
            'class' => array('nav', 'navbar-nav', 'secondary-links', 'navbar-right'),
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
      </div>

    </div>
  </nav>
</div>

