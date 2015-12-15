var x = 1;
x = 2;
x = 3
2
3


funcaoum(); -> 2;
funcaodois(); -> 3;


function funcaoum() {
    setTimeout(function{
        x = 3;
    }, 300);

    return 3;
}

function funcaodois() {
    return 3;
}
