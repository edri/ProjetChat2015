INSERT INTO user (idUser,login, firstName, lastName, password, isConnected, publicKey, privateKey, salt, masterKey)
	VALUES	(1,'jacquesM', 'Jacques','Métrailler', 'pass', 1, 'asdasd', 'asdsad', 0101010, 01010101010),
			(2,'JeanT', 'Jean','Toto', 'pass', 0, 'asdasd', 'asdsad', 0101010, 01010101010),
			(3,'JeanMich', 'Jean-Michel', 'Dupont', 'ok', 1, 'a123','asdad', 01010111, 00000101),
			(4,'B2B', 'Bastien', 'Rouiller', 'passw', 1, 'aiajidj', 'aisdj2', 0100100, 01010101),
			(5,'edri', 'Miguel', 'Santamaria', 'secret', 0, 'a19jd', 'aisjdi1j', 0101001100, 01001010),
			(6,'titi', 'Mélanie', 'Huck', 'rrhok', 0, 'a91dj9', '129jd9', 010101000, 000001001),
			(7,'licorne', 'Benoist', '', 'java', 1, 'ij9as', '19dj12', 0101010, 01000100),
			(8,'janP', 'Jan', 'Purro', 'ohoui', 0, 'adj1', '1d0j12', 01001010, 0101001);
			
INSERT INTO room(idRoom, name, limitOfStoredMessages, private, visible)
	VALUES	(1,'Salle principale', 5, 0,NULL),
			(2,'Salle secrète', 10, 1, 0),
			(3,'Salle de bain', 20, 0,NULL),
			(4,'Salle privée', 15, 1,1);
		
INSERT INTO privilege
	VALUES	(1, 'user'),
			(2, 'admin');
			
INSERT INTO roommembership(idUser, idRoom, idPrivilege)
	VALUES	(1,1,2),
			(2,1,1),
			(3,1,1),
			(4,1,1),
			(5,1,1),
			(6,1,1),
			(7,1,2),
			(8,1,1),
			(7,2,1),
			(8,2,2);
			
INSERT INTO message(contents, date, idUser, idRoom)
	VALUES 	('Coucou', datetime('NOW'), 1, 1),
			("Est-ce qu'il y a quelqu'un ?", datetime('NOW'), 1,1),
			('Oui moi', datetime('NOW'), 2, 1),
			("Le Java, c'est trop bien !", datetime('NOW'), 4, 1),
			("Tu vas le payer !", datetime('NOW'), 7, 1),
			('On est trop d4rk', datetime('NOW'), 7, 2),
			('Ouai klayr', datetime('NOW'), 8, 2);
			
INSERT INTO banning(dateFrom, dateUntil, note, idUser, idRoom)
	VALUES 	(datetime('NOW'), '2022-03-27', 'Tu es banni à jamais !', 4, 1);
			
