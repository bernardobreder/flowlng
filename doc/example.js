
function a() do
    return 1
end

function b() do
    return a()*2
end

function a() do
    return 2
end

var a = 1
var b = 3
var c = a + b

return 1 + "a" + "b" + b() + c