["0","1","X","Y","BLANK"]

WHILE ["0","1"]

	IF ["0"]
		WRITE ["Y"]
		WHILE ["0","X"]
			MOVERIGHT
		END
		
		IF ["BLANK"]
			REJECT
		END

		WRITE ["X"]
		WHILE ["0","X"]
			MOVELEFT
		END

		IF ["BLANK"]
			REJECT
		END
		
		WRITE ["X"]
		MOVERIGHT
	END

	IF ["1"]
		WRITE ["Y"]
		WHILE ["1","X"]
			MOVERIGHT
		END
		
		IF ["BLANK"]
			REJECT
		END

		WRITE ["X"]
		WHILE ["1","X"]
			MOVELEFT
		END

		IF ["BLANK"]
			REJECT
		END
		
		WRITE ["X"]
		MOVERIGHT
	END
END

IF ["BLANK"]
	ACCEPT
END

