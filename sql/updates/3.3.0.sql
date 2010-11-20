DROP TABLE IF EXISTS `modelista`;
CREATE TABLE `modelista` (
  `id` int(10) unsigned NOT NULL auto_increment,
  `nick` varchar(100) NOT NULL default '',
  `channel` varchar(100) NOT NULL default '',
  `rang` varchar(10) NOT NULL default '',
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=1 DEFAULT CHARSET=latin1;

REPLACE `schumix` VALUES ('11', '', 'mode', 'ki');
ALTER TABLE channel CHANGE column `funkciok` `funkciok` varchar(255) NOT NULL default ',koszones:be,log:be,rejoin:be,parancsok:be,hl:be,kick:be,mode:be';
UPDATE channel SET funkciok = ',koszones:be,log:be,rejoin:be,parancsok:be,hl:be,kick:be,mode:be';
