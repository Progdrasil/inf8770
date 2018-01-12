# Question 01: Hypotheses
1. Pour un texte long LZW aura un taux de compression plus eleve que huffman, par contre si le texte est un ensemble aleatoire de symbole ou un texte cours huffman sera plus eleve.
2. Le temps de compression est proportionel aux taux de compression, donc dans les cas ou un codage a un taux plus elever plus la compression sera rapide.
3. Le taux de compression d'images brouillé sera plus eleve pour le codage huffman que le codage LZW.
4. Le taux de compression d'images uniforme ou avec peu de couleurs tel que des identicons, LZW aurat un mailleur taux.
5. Le taux de compression de GIFs sera plus elever pour le codage LZW que le codage huffman.
6. Le temps de decompression est aussi proportionel au taux de compression. C'est a dire qu'un fichier avec un haut taux de compression sera plus rapide a decoder.

# Question 02: Methodologie
### La methode
* Pour chaque fichier dans notre base de donnees on va compresser avec les deux types de codages, c'est a dire Huffman et LZW, et comparer le taux de compression et la vitesse de compression pour les deux fichiers en output

### La base de donnees
* on aura des fichiers de textes avec differente longueurs de deux types:
	* avec du texte comprehensible
	* avec un ensemble aleatoire de symboles
* On aura des images de differente tailes de trois types:
	* Des photographes noir et blanc
	* Des images monochrome
	* Des images type 8-bit (identicons, 8bit art, etc)
* On aura des GIFs de differentes longueur.

### Les criteres
* Les codes serons comparer selon le taux de compression, celui avec le taux le plus elever est considere meilleur pour cette tache.
* Les codes serons aussi comparer selon la vitesse a laquelle ils sont compresser et decompresser. Celui qui est le plus rapide au deux taches est considere meilleur pour cette tache

### code informatique utilise
Language informatique?
* On utilise le c++ comme language pour la rapiditer de traitement de donnees.

# Question 03
CODE ALL THE THINGS!!!

# Question 04
ANALYSE ALL THE THINGS!!!


pour git clone fait cette commande:
``` bash
git submodule init --quiet
```
