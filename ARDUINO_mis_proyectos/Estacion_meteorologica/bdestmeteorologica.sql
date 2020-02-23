-- phpMyAdmin SQL Dump
-- version 4.5.4.1deb2ubuntu2
-- http://www.phpmyadmin.net
--
-- Host: localhost
-- Generation Time: Aug 25, 2016 at 08:15 PM
-- Server version: 5.7.13-0ubuntu0.16.04.2
-- PHP Version: 7.0.8-0ubuntu0.16.04.2

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `bdestmeteorologica`
--

-- --------------------------------------------------------

--
-- Table structure for table `tb_datos`
--

CREATE TABLE `tb_datos` (
  `FechaHora` datetime DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  `tiempoArduino` int(20) NOT NULL,
  `Temperatura` float NOT NULL,
  `Presion` float NOT NULL,
  `Humedad` float NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `tb_datos`
--

INSERT INTO `tb_datos` (`FechaHora`, `tiempoArduino`, `Temperatura`, `Presion`, `Humedad`) VALUES
('2016-08-25 19:42:21', 4819629, 28.69, 0, 0),
('2016-08-25 19:42:24', 4822336, 28.69, 0, 0),
('2016-08-25 19:42:27', 4825822, 28.69, 0, 0),
('2016-08-25 19:42:29', 4827747, 28.69, 0, 0),
('2016-08-25 19:42:36', 4834302, 28.69, 0, 0),
('2016-08-25 19:42:37', 4836229, 28.69, 0, 0),
('2016-08-25 19:42:39', 4838153, 28.69, 0, 0),
('2016-08-25 19:48:31', 5189855, 28.75, 0, 0);

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
