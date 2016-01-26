*shake*
	MOVELEFT
	MOVERIGHT
END

["a","b","c","d"]

WHILE ["a","b"]
	_shake_
	ACCEPT
END

IF ["a"]
	MOVERIGHT
END

IF ["b"]
	ACCEPT
END

REJECT
