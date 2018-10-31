<?php
/**
 * @file
 * Template file for single item list item.
 */
$alt = check_plain($title . ' ' . $author);
?>
<div class="ding-item-list-item">
  <div class="item-cover">
    <a href="/ting/object/<?php print $faust; ?>">
      <img src="<?php print $cover; ?>" alt="<?php echo $alt; ?>" />
    </a>
  </div>
  <div class="item-details">
    <div class="item-title"><a href="/ting/object/<?php print $faust; ?>"><?php print $title; ?></a></div>
      <?php if (!empty($author)): ?>
        <div class="item-author"><?php print t('By @author', array('@author' => $author)); ?> (<?php print $year;?>)</div>
      <?php endif; ?>
      <?php if (!empty($loan_form)): ?>
        <div class="item-loan"><?php print $loan_form; ?></div>
      <?php endif; ?>
    <?php if (!empty($description)): ?>
        <div class="item-description"><?php print $description; ?></div>
    <?php endif; ?>
  </div>
</div>
