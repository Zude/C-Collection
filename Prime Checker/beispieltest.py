#!/usr/bin/env python

suite = [
    Test (
        name = "Hilfeausgabe",
        description = "Der Hilfetext muss auf stdout ausgegeben werden.",
        command = "$DUT -h",
        stdout = ExpectFile("usage.txt"),
        stderr = "",
        returnCode = 0,
        timeout = 10
    ),
    Test (
        name = "Primzahltest mit Primzahl",
        description = "Ist 7 eine Primzahl?",
        command = "$DUT 7 =",
        stdout = "*\o/* The number 7 is a prime number. *\o/*$n",
        stderr = "",
        returnCode = "0",
        timeout = 10
    ),
    Test (
        name = "Primzahltest mit Primzahl (Variante 2)",
        description = "Ist 7 eine Primzahl?",
        command = "$DUT 7 =",
        stdout = ExpectFile("seven_prime.txt"),
        stderr = "",
        returnCode = "0",
        timeout = 10
    ),
    Test (
        name = "Fehlerfall: keine Parameter",
        description = "Fehlermeldung muss mit 'Error:' beginnen und auf stderr stehen",
        command = "$DUT",
        stdout = "",
        stderr = "regex:^Error:",
        returnCode = lambda v: v !=0,
        timeout = 10
    ),
]
