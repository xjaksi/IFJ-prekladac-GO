#!/bin/bash

for d in ./sem/*/
do
     for f in $d*.go
     do
        file=$( echo "$(basename $f)" | cut -d'.' -f1 )
        
        head=$( echo $(head -n 1 $f) | cut -d " " -f1 )

        code=$( echo $(head -n 1 $f) | cut -d " " -f2 )

        errHead="//compiler"
        okHead="//"
        if [ "$head" = "$errHead" ]
        then                
            cp $f ./semantics/${code}/${file}.err
        fi
        if [ "$head" = "$okHead" ]
        then
            cp $f ./semantics/${file}.ok
        fi
    done
done

for d in ./syn/*/
do
     for f in $d*.go
     do
        file=$( echo "$(basename $f)" | cut -d'.' -f1 )
        
        head=$( echo $(head -n 1 $f) | cut -d " " -f1 )

        errHead="//compiler"
        okHead="//"
        if [ "$head" = "$errHead" ]
        then                
            cp $f ./syntax/${file}.err
        fi
        if [ "$head" = "$okHead" ]
        then
            cp $f ./syntax/${file}.ok
        fi
    done
done

for d in ./lex/*/
do
     for f in $d*.go
     do
        file=$( echo "$(basename $f)" | cut -d'.' -f1 )
        
        head=$( echo $(head -n 1 $f) | cut -d " " -f1 )

        errHead="//compiler"
        okHead="//"
        if [ "$head" = "$errHead" ]
        then                
            cp $f ./lexical/${file}.err
        fi
        if [ "$head" = "$okHead" ]
        then
            cp $f ./lexical/${file}.ok
        fi
    done
done