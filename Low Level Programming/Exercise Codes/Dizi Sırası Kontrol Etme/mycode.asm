myStack segment para stack 'stack'
    dw  20  dup(?)
myStack ends

myData segment para 'data'
    dizi    db  1,3,4,5,4,7,8,9,10
    elemanSayisi   dw   9
    kontrol db  ?   
    S       db  0
myData ends

myCode segment para 'code'
    assume cs:myCode,ss:myStack,ds:myData
MAIN proc far
    push ds
    xor ax,ax
    push ax
    mov ax,myData
    mov ds,ax
    xor SI,SI
    mov bx,0
    mov cx,elemanSayisi 
    dec cx
l1: cmp bx,0
    jne whileCikis
    cmp SI,cx
    jge whileCikis
    mov al,dizi[SI]
    cmp al,dizi[SI+1]
    jle loopDevam 
    mov bx,1
loopDevam:inc SI 
    loop l1    
whileCikis:cmp bx,0
    jne bitis
    mov S,1                   
    



bitis:
    mov al,S
    retf  
    
MAIN    endp
myCode  ends
        end MAIN
       
    
    