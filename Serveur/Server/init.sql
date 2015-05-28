CREATE TABLE IF NOT EXISTS user (
  idUser INTEGER PRIMARY KEY AUTOINCREMENT,
  login VARCHAR(16) NOT NULL,
  firstName VARCHAR(32),
  lastName VARCHAR(32),
  password BINARY(64) NOT NULL,
  profilePicture VARCHAR(16),
  isConnected TINYINT(1) NOT NULL,
  lastConnection DATETIME,
  publicKey VARBINARY NOT NULL,
  privateKey VARBINARY NOT NULL,
  saltPassword BINARY(8) NOT NULL,
  saltKey BINARY(8) NOT NULL
  );



CREATE TABLE IF NOT EXISTS room (
  idRoom INTEGER PRIMARY KEY AUTOINCREMENT,
  name VARCHAR(32) NOT NULL,
  limitOfStoredMessages INT NOT NULL,
  private TINYINT(1) NOT NULL,
  visible TINYINT(1),
  picture VARCHAR(16)
  );


CREATE TABLE IF NOT EXISTS privilege (
  idPrivilege INTEGER PRIMARY KEY AUTOINCREMENT,
  name VARCHAR(16) NOT NULL
  );
  
INSERT INTO privilege (name)
	VALUES	('user'),
			('admin'),
			('request');


CREATE TABLE IF NOT EXISTS message (
  idMessage INTEGER PRIMARY KEY AUTOINCREMENT,
  contents BLOB NOT NULL,
  date DATETIME NOT NULL,
  idUser INT NOT NULL,
  idRoom INT NOT NULL,
  lastUpdated DATETIME,
    FOREIGN KEY (idRoom)
    REFERENCES room (idRoom)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
    FOREIGN KEY (idUser)
    REFERENCES user (idUser)
);


CREATE TABLE IF NOT EXISTS roomMembership (
  idRoomMembership INTEGER PRIMARY KEY AUTOINCREMENT,
  idUser INT NOT NULL,
  idRoom INT NOT NULL,
  idPrivilege INT NOT NULL,
  roomKey VARBINARY,
    FOREIGN KEY (idUser)
    REFERENCES user (idUser),
    FOREIGN KEY (idRoom)
    REFERENCES room (idRoom),
    FOREIGN KEY (idPrivilege)
    REFERENCES privilege (idPrivilege)
  );


CREATE TABLE IF NOT EXISTS banning (
  idBanning INTEGER PRIMARY KEY AUTOINCREMENT,
  dateFrom DATETIME NOT NULL,
  dateUntil DATETIME,
  note LONGTEXT,
  idUser INT NOT NULL,
  idRoom INT NOT NULL,
    FOREIGN KEY (idUser)
    REFERENCES user (idUser),
    FOREIGN KEY (idRoom)
    REFERENCES room (idRoom)
	);
