myStack segment para stack 'stack'
    dw  20  (?)
myStack ends
myData  segment para 'data'
studs       dw 280
kalan       dw 0
but         dw 0
basarili    dw 0
topNot      dw 0
esik        db 40     
notlar      db 280 dup(?)
myData  ends 
myCode  segment para 'code'
MAIN    proc far 
        push ds
        xor ax,ax
        mov ax,myData
        mov ds,ax 
        
    
        xor SI,SI
        mov cx,studs
l1:     mov al,notlar[SI]
        cmp al,esik
        jae esikustu
        inc kalan
        jmp d1
esikustu:cbw
        add topNot,ax
d1:     inc SI
        loop l1
        mov cx,studs
        sub cx,kalan
        xor dx,dx
        mov ax,topNot
        div cx 
        xor SI,SI
        mov cx,studs
l2:     mov ah,notlar[SI]
        cmp al,ah
        jbe gecti
        cmp ah,esik
        jb kaldi
        inc but 
        jmp kaldi
gecti:  inc basarili
kaldi:  inc SI
        loop l2
 
        
        retf
MAIN    endp
myCode  ends
        end MAIN

