<?php
/**
 * @file
 * Template for Kalatheme Reynolds.
 *
 * Variables:
 * - $css_id: An optional CSS id to use for the layout.
 * - $content: An array of content, each item in the array is keyed to one
 * panel of the layout. This layout supports the following sections:
 */
?>

<div class="panel-display reynolds clearfix <?php if (!empty($class)) { print $class; } ?>" <?php if (!empty($css_id)) { print "id=\"$css_id\""; } ?>>

  <section class='section alt' id='promo'>
    <div class='container'>
      <div class="reynolds-container reynolds-header clearfix panel-panel">
        <div class="reynolds-container-inner reynolds-header-inner panel-panel-inner">
          <?php print $content['header']; ?>
        </div>
      </div>
    </div>
  </section>

  <section class='section'>
    <div class='container'>
      <div class='row-fluid'>
        <div class="reynolds-container reynolds-column-content clearfix">
          <div class="reynolds-column-content-region reynolds-sidebar panel-panel span3">
            <div class="reynolds-column-content-region-inner reynolds-sidebar-inner panel-panel-inner">
              <?php print $content['sidebar']; ?>
            </div>
          </div>
          <div class="reynolds-column-content-region reynolds-content panel-panel span9">
            <div class="reynolds-column-content-region-inner reynolds-content-inner panel-panel-inner">
              <?php print $content['contentmain']; ?>
            </div>
          </div>
        </div>
      </div>
    </div>
  </section>
  
  <footer class='section' id='footer' role='contentinfo'>
    <div class='container'>
      <div class="reynolds-container reynolds-column-content reynolds-column-content-row1 clearfix row-fluid">
        <div class="reynolds-column-content-region reynolds-column reynolds-column1 panel-panel span4">
          <div class="reynolds-column-content-region-inner reynolds-column-inner reynolds-column1-inner panel-panel-inner">
            <?php print $content['footer1']; ?>
          </div>
        </div>
        <div class="reynolds-column-content-region reynolds-column reynolds-column2 panel-panel span4">
          <div class="reynolds-column-content-region-inner reynolds-column-inner reynolds-column2-inner panel-panel-inner">
            <?php print $content['footer2']; ?>
          </div>
        </div>
        <div class="reynolds-column-content-region reynolds-column reynolds-column3 panel-panel span4">
          <div class="reynolds-column-content-region-inner reynolds-column-inner reynolds-column3-inner panel-panel-inner">
            <?php print $content['footer3']; ?>
          </div>
        </div>
      </div>
    </div>
  </footer>
  
</div><!-- /.reynolds -->
