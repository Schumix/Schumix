DROP TABLE IF EXISTS `kicklista`;
CREATE TABLE `kicklista` (
  `id` int(10) unsigned NOT NULL auto_increment,
  `nick` varchar(100) NOT NULL default '',
  `channel` varchar(100) NOT NULL default '',
  `oka` text NOT NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=75 DEFAULT CHARSET=latin1;

REPLACE `schumix` VALUES ('10', '', 'kick', 'ki');
ALTER TABLE channel CHANGE column `funkciok` `funkciok` varchar(255) NOT NULL default ',koszones:be,log:be,rejoin:be,parancsok:be,hl:be,kick:be';
UPDATE channel SET funkciok = ',koszones:be,log:be,rejoin:be,parancsok:be,hl:be,kick:be';
