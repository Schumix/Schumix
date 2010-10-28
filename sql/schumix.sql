DROP TABLE IF EXISTS `adminok`;
CREATE TABLE `adminok` (
  `id` int(10) unsigned NOT NULL auto_increment,
  `nev` varchar(21) NOT NULL default '',
  `jelszo` varchar(100) NOT NULL default '',
  `ip` varchar(100) NOT NULL default '',
  `flag` int(10) NOT NULL DEFAULT '0',
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=75 DEFAULT CHARSET=latin1;

DROP TABLE IF EXISTS `channel`;
CREATE TABLE `channel` (
  `id` int(10) unsigned NOT NULL auto_increment,
  `funkciok` varchar(255) NOT NULL default ',koszones:be,log:be,rejoin:be,parancsok:be,hl:be',
  `szoba` text NOT NULL default '',
  `jelszo` text NOT NULL default '',
  `aktivitas` text NOT NULL default '',
  `error` text NOT NULL default '',
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=75 DEFAULT CHARSET=latin1;

-- ----------------------------
-- Records 
-- ----------------------------
INSERT INTO `channel` VALUES ('1', ',koszones:be,log:be,rejoin:be,parancsok:be,hl:be', '#schumix', '', '', '');

DROP TABLE IF EXISTS `gitinfo`;
CREATE TABLE `gitinfo` (
  `id` int(10) unsigned NOT NULL auto_increment,
  `nev` varchar(21) NOT NULL default '',
  `tipus` varchar(21) NOT NULL default '',
  `link` varchar(255) NOT NULL default '',
  `oldal` text NOT NULL default '',
  `engedely` int(10) unsigned NOT NULL,
  `account` varchar(25) NOT NULL default '',
  `password` varchar(25) NOT NULL default '',
  `channel` text NOT NULL default '',
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=75 DEFAULT CHARSET=latin1;

DROP TABLE IF EXISTS `hginfo`;
CREATE TABLE `hginfo` (
  `id` int(10) unsigned NOT NULL auto_increment,
  `nev` varchar(21) NOT NULL default '',
  `link` varchar(255) NOT NULL default '',
  `oldal` text NOT NULL default '',
  `engedely` int(10) unsigned NOT NULL,
  `account` varchar(25) NOT NULL default '',
  `password` varchar(25) NOT NULL default '',
  `channel` text NOT NULL default '',
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=75 DEFAULT CHARSET=latin1;

DROP TABLE IF EXISTS `hluzenet`;
CREATE TABLE `hluzenet` (
  `id` int(10) unsigned NOT NULL auto_increment,
  `nick` varchar(100) NOT NULL default '',
  `info` text NOT NULL,
  `alapot` varchar(10) NOT NULL default '',
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=75 DEFAULT CHARSET=latin1;

DROP TABLE IF EXISTS `irc_parancsok`;
CREATE TABLE `irc_parancsok` (
  `guid` int(10) unsigned NOT NULL auto_increment,
  `parancs` varchar(21) NOT NULL default '',
  `hasznalata` text NOT NULL default '',
  PRIMARY KEY  (`guid`)
) ENGINE=MyISAM AUTO_INCREMENT=3 DEFAULT CHARSET=latin1;

-- ----------------------------
-- Records 
-- ----------------------------
INSERT INTO `irc_parancsok` VALUES ('1', 'rang', 'Rang hasznalata: /mode <channel> <rang> <nev>');
INSERT INTO `irc_parancsok` VALUES ('2', 'rang1', 'Rang mentese: /chanserv <rang (sop, aop, hop, vop)> <channel> ADD <nev>');
INSERT INTO `irc_parancsok` VALUES ('3', 'nick', 'Nick csere hasznalata: /nick <uj nev>');
INSERT INTO `irc_parancsok` VALUES ('4', 'kick', 'Kick hasznalata: /kick <channel> <nev> (<oka> nem feltetlen kell)');
INSERT INTO `irc_parancsok` VALUES ('5', 'owner', 'Ownermod hasznalata: /msg chanserv SET <channel> ownermode on ');

DROP TABLE IF EXISTS `jegyzetek`;
CREATE TABLE `jegyzetek` (
  `id` int(10) unsigned NOT NULL auto_increment,
  `kod` text NOT NULL default '',
  `nick` varchar(21) NOT NULL default '',
  `jegyzet` text NOT NULL default '',
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=3 DEFAULT CHARSET=latin1;

DROP TABLE IF EXISTS `schumix`;
CREATE TABLE `schumix` (
  `entry` int(10) unsigned NOT NULL auto_increment,
  `irc_cim` varchar(21) NOT NULL default '',
  `funkcio_nev` varchar(21) NOT NULL default '',
  `funkcio_status` varchar(10) NOT NULL default '',
  PRIMARY KEY  (`entry`)
) ENGINE=MyISAM AUTO_INCREMENT=75 DEFAULT CHARSET=latin1;

-- ----------------------------
-- Records 
-- ----------------------------
INSERT INTO `schumix` VALUES ('1', '', 'reconnect', 'be');
INSERT INTO `schumix` VALUES ('2', '', 'koszones', 'be');
INSERT INTO `schumix` VALUES ('3', '', 'log', 'be');
INSERT INTO `schumix` VALUES ('4', '', 'svn', 'ki');
INSERT INTO `schumix` VALUES ('5', '', 'git', 'ki');
INSERT INTO `schumix` VALUES ('6', '', 'hg', 'ki');
INSERT INTO `schumix` VALUES ('7', '', 'rejoin', 'be');
INSERT INTO `schumix` VALUES ('8', '', 'parancsok', 'be');
INSERT INTO `schumix` VALUES ('9', '', 'hl', 'be');

DROP TABLE IF EXISTS `svninfo`;
CREATE TABLE `svninfo` (
  `id` int(10) unsigned NOT NULL auto_increment,
  `nev` varchar(21) NOT NULL default '',
  `link` varchar(255) NOT NULL default '',
  `oldal` text NOT NULL default '',
  `engedely` int(10) unsigned NOT NULL,
  `account` varchar(25) NOT NULL default '',
  `password` varchar(25) NOT NULL default '',
  `channel` text NOT NULL default '',
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=75 DEFAULT CHARSET=latin1;

-- ----------------------------
-- Records 
-- ----------------------------
INSERT INTO `svninfo` VALUES ('1', 'Schumix', 'https://www.assembla.com/spaces/schumix/stream.rss', 'assembla', '0', '', '', ',#schumix');

DROP TABLE IF EXISTS `sznap`;
CREATE TABLE `sznap` (
  `guid` int(10) unsigned NOT NULL auto_increment,
  `nev` text NOT NULL,
  `honap` varchar(21) NOT NULL default '',
  `honap1` tinyint(3) unsigned NOT NULL,
  `nap` tinyint(3) unsigned NOT NULL,
  PRIMARY KEY  (`guid`)
) ENGINE=MyISAM AUTO_INCREMENT=75 DEFAULT CHARSET=latin1;

DROP TABLE IF EXISTS `uptime`;
CREATE TABLE `uptime` (
  `entry` int(100) unsigned NOT NULL auto_increment,
  `datum` text NOT NULL,
  `uptime` text NOT NULL,
  `memory` text NOT NULL,
  PRIMARY KEY  (`entry`)
) ENGINE=MyISAM AUTO_INCREMENT=75 DEFAULT CHARSET=latin1;
