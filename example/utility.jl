
function kl_sdistance(p, q)
    if size(p, 1) != size(q, 1) 
        println("p and q should be the same size.")
        return -1
    end

    ans = 0.0
    for i = 1:size(p,1)
        ans += p[i] * log(p[i]/q[i])
    end
    ans
end
