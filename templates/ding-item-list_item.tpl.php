<?php
/**
 * @file
 *
 * Template file for single item list item.
 */
?>

<div class="ding-item-list-item">
  <div class="item-cover">
    <img src="<?php print $cover; ?>" alt="" />
  </div>
  <div class="item-details">
    <p class="item-title"><?php print $title; ?></p>
    <p class="item-author"><?php print t('By');?> <?php print $author; ?> (<?php print $year;?>)</p>
    <p class="item-rating"><?php print $rating; ?> (<?php print $rating_count; ?>)</p>
    <p class="item-reviews">(<?php print $review_count;?>) <?php print t('reviews'); ?></p>
    <p class="item-loan"></p>
  </div>
</div>
