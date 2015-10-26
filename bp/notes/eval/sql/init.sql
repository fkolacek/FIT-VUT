/*
SQLyog Community v11.27 (64 bit)
MySQL - 5.5.40-MariaDB : Database - wavsep
*********************************************************************
*/

/*!40101 SET NAMES utf8 */;

/*!40101 SET SQL_MODE=''*/;

/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;
CREATE DATABASE /*!32312 IF NOT EXISTS*/`wavsep` /*!40100 DEFAULT CHARACTER SET utf8 COLLATE utf8_czech_ci */;

USE `wavsep`;

/*Table structure for table `attacks` */

DROP TABLE IF EXISTS `attacks`;

CREATE TABLE `attacks` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `type` varchar(32) COLLATE utf8_czech_ci DEFAULT NULL,
  `category` varchar(256) COLLATE utf8_czech_ci DEFAULT NULL,
  `subcategory` varchar(256) COLLATE utf8_czech_ci DEFAULT NULL,
  `name` varchar(256) COLLATE utf8_czech_ci DEFAULT NULL,
  `link` varchar(512) COLLATE utf8_czech_ci DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_czech_ci;

/*Data for the table `attacks` */

/*Table structure for table `hits` */

DROP TABLE IF EXISTS `hits`;

CREATE TABLE `hits` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `scannerID` int(11) DEFAULT NULL,
  `attackID` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_czech_ci;

/*Data for the table `hits` */

/*Table structure for table `tests` */

DROP TABLE IF EXISTS `tests`;

CREATE TABLE `tests` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `scanner` varchar(64) COLLATE utf8_czech_ci DEFAULT NULL,
  `date` datetime DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_czech_ci;

/*Data for the table `tests` */

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;
