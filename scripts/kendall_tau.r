# Kendall's tau-c

# https://stackoverflow.com/a/2567869/4603385
# Number of concordant pairs.
P = function(t) {
  r_ndx = row(t)
  c_ndx = col(t)
  sum(t * mapply(function(r, c){sum(t[(r_ndx > r) & (c_ndx > c)])},
                 r = r_ndx, c = c_ndx))
}

# Number of discordant pairs.
Q = function(t) {
  r_ndx = row(t)
  c_ndx = col(t)
  sum(t * mapply( function(r, c){
    sum(t[(r_ndx > r) & (c_ndx < c)])
  },
  r = r_ndx, c = c_ndx) )
}

kendall_tau_c = function(t){
  t = as.matrix(t) 
  m = min(dim(t))
  n = sum(t)
  ks_tauc = (m * 2 * (P(t) - Q(t))) / ((n ^ 2) * (m - 1))
  ks_tauc
}
