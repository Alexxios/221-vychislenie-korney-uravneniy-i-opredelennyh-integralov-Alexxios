section .data
    const1 dq 3.0
    const2 dq 0.5

section .text

global f1
f1:
    push ebp
    mov ebp, esp
    
    fld qword [ebp + 8]
    fld1
    fsubp                       ; st0 := x-1
    fld st0                     
    fmulp                       ; st0 := (x-1)^2
    fld1
    faddp                       ; st0 += 1
    fld qword [const1]          ; st1 <- st0, st0 <- 3.0
    fxch                        ; st0 <-> st1
    fdivp                       ; st0 := st1 / st0

    pop ebp
    ret

global f2
f2:
    push ebp
    mov ebp, esp

    fld qword [const2]
    fld qword [ebp + 8]
    faddp
    fsqrt
    
    pop ebp
    ret
    
global f3
f3:
    push ebp                    
    mov ebp, esp

    fldz
    fld qword [ebp + 8]         
    fsubp                       ; st0 := -x
    fldl2e
    fmulp                       ; st0 := log2(e) * (-x)
    fld1
    fld st1                     
    fprem                       ; st0 := partial remainder((log2(e) * (-x)) / 1) <- name it R
    f2xm1                       ; st0 := 2^R - 1
    faddp                       ; st0 += 1
    fscale                      ; st0 := 2^R * 2^(trunc(log2(e) * (-x)))
    fstp st1

    pop ebp
    ret