
function a() do
    return 1
end

function b() do
    return a()*2
end

function a() do
    return 2
end

var aa = 1
var bb = 3
var cc = aa + bb

//return false or false or true
return 2 + "a" + "b" + b() + cc
