<?php
/**
 * @file
 * Template for Kalatheme mcbryan.
 *
 * Variables:
 * - $css_id: An optional CSS id to use for the layout.
 * - $content: An array of content, each item in the array is keyed to one
 * panel of the layout. This layout supports the following sections:
 */
?>

<div class="panel-display mcbryan clearfix <?php !empty($class) ? print $class : ''; ?>" <?php !empty($css_id) ? print "id=\"$css_id\"" : ''; ?>>
    
    <div class='container'>
      <div class="mcbryan-container mcbryan-header clearfix row-fluid">
		    <div class="mcbryan-header mcbryan-main panel-panel span7">
		      <div class="mcbryan-header-inner mcbryan-main-inner panel-panel-inner">
	          <?php print $content['main']; ?>
	        </div>
        </div>
		    <div class="mcbryan-header mcbryan-image panel-panel span5">
		      <div class="mcbryan-header-inner mcbryan-image-inner panel-panel-inner">
	          <?php print $content['image']; ?>
	        </div>
	      </div>
      </div>
    </div>
</div><!-- /.mcbryan -->
