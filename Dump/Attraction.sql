-- --------------------------------------------------------
-- Хост:                         localhost
-- Версия сервера:               5.6.26 - MySQL Community Server (GPL)
-- ОС Сервера:                   Win32
-- HeidiSQL Версия:              9.3.0.4984
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

-- Дамп структуры базы данных SunCity
CREATE DATABASE IF NOT EXISTS `SunCity` /*!40100 DEFAULT CHARACTER SET utf8 */;
USE `SunCity`;


-- Дамп структуры для таблица SunCity.active_bracers
CREATE TABLE IF NOT EXISTS `active_bracers` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `code` int(11) NOT NULL,
  `enter_time` datetime NOT NULL,
  `enter_number` tinyint(4) NOT NULL,
  `cash` decimal(10,2) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Дамп данных таблицы SunCity.active_bracers: ~0 rows (приблизительно)
DELETE FROM `active_bracers`;
/*!40000 ALTER TABLE `active_bracers` DISABLE KEYS */;
/*!40000 ALTER TABLE `active_bracers` ENABLE KEYS */;


-- Дамп структуры для таблица SunCity.active_transactions
CREATE TABLE IF NOT EXISTS `active_transactions` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `product_id` int(11) NOT NULL,
  `quantity` int(11) NOT NULL DEFAULT '1',
  `total_price` decimal(10,2) NOT NULL,
  `active_bracers_id` int(11) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `FK_active_transactions_active_bracers` (`active_bracers_id`),
  KEY `FK_active_transactions_products` (`product_id`),
  CONSTRAINT `FK_active_transactions_active_bracers` FOREIGN KEY (`active_bracers_id`) REFERENCES `active_bracers` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `FK_active_transactions_products` FOREIGN KEY (`product_id`) REFERENCES `products` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Дамп данных таблицы SunCity.active_transactions: ~0 rows (приблизительно)
DELETE FROM `active_transactions`;
/*!40000 ALTER TABLE `active_transactions` DISABLE KEYS */;
/*!40000 ALTER TABLE `active_transactions` ENABLE KEYS */;


-- Дамп структуры для таблица SunCity.bracers_history
CREATE TABLE IF NOT EXISTS `bracers_history` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `code` int(11) NOT NULL,
  `enter_time` datetime NOT NULL,
  `enter_number` tinyint(4) NOT NULL,
  `cash` decimal(10,2) NOT NULL,
  `exit_time` datetime NOT NULL,
  `exit_number` tinyint(4) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- Дамп данных таблицы SunCity.bracers_history: ~0 rows (приблизительно)
DELETE FROM `bracers_history`;
/*!40000 ALTER TABLE `bracers_history` DISABLE KEYS */;
/*!40000 ALTER TABLE `bracers_history` ENABLE KEYS */;


-- Дамп структуры для таблица SunCity.products
CREATE TABLE IF NOT EXISTS `products` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `title` varchar(50) NOT NULL,
  `price` decimal(10,2) NOT NULL,
  `product_types_id` int(11) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `FK_products_product_types` (`product_types_id`),
  CONSTRAINT `FK_products_product_types` FOREIGN KEY (`product_types_id`) REFERENCES `product_types` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Дамп данных таблицы SunCity.products: ~0 rows (приблизительно)
DELETE FROM `products`;
/*!40000 ALTER TABLE `products` DISABLE KEYS */;
/*!40000 ALTER TABLE `products` ENABLE KEYS */;


-- Дамп структуры для таблица SunCity.product_types
CREATE TABLE IF NOT EXISTS `product_types` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `title` varchar(50) NOT NULL,
  `mode` enum('Park','Bar') NOT NULL DEFAULT 'Park',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Дамп данных таблицы SunCity.product_types: ~0 rows (приблизительно)
DELETE FROM `product_types`;
/*!40000 ALTER TABLE `product_types` DISABLE KEYS */;
/*!40000 ALTER TABLE `product_types` ENABLE KEYS */;


-- Дамп структуры для таблица SunCity.transactions_history
CREATE TABLE IF NOT EXISTS `transactions_history` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `product_id` int(11) NOT NULL DEFAULT '1',
  `quantity` int(11) NOT NULL DEFAULT '1',
  `total_price` decimal(10,2) NOT NULL,
  `bracers_history_id` int(11) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `FK_active_transactions_active_bracers` (`bracers_history_id`),
  KEY `FK_transactions_history_products` (`product_id`),
  CONSTRAINT `FK_transactions_history_products` FOREIGN KEY (`product_id`) REFERENCES `products` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `transactions_history_ibfk_1` FOREIGN KEY (`bracers_history_id`) REFERENCES `bracers_history` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- Дамп данных таблицы SunCity.transactions_history: ~0 rows (приблизительно)
DELETE FROM `transactions_history`;
/*!40000 ALTER TABLE `transactions_history` DISABLE KEYS */;
/*!40000 ALTER TABLE `transactions_history` ENABLE KEYS */;
/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
