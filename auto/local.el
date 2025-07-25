;; -*- lexical-binding: t; -*-

(TeX-add-style-hook
 "local"
 (lambda ()
   (LaTeX-add-bibitems
    "5BComput48:online"
    "geuvers2014church"
    "DBLP:conf/ifl/KoopmanPJ14"
    "DBLP:conf/birthday/Jansen13"
    "DBLP:conf/sfp/JansenKP06"
    "jacobs1997tutorial"
    "downen2019codata"
    "DBLP:conf/esop/DownenSAJ19"))
 '(or :bibtex :latex))

