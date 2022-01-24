
fname = "data.bin"

fstat  = stat(fname)
Nbytes = 2
Nj     = Int(fstat.size/Nbytes)
Vint   = Vector{Int}(undef, Nj)

file = open(fname,"r")
  for j=1:Nj
    Vint[j] = Int(read(file, UInt16))
  end
close(file)

M = reshape(Vint, 100, Int(Nj/100))'

Vsum = sum(M[:, 2:end], dims=2)

sum(Vsum .== sum(0:99))/size(M, 1)
all(Vsum .== sum(0:99))

Vi = M[1, :]

all(diff(Vi) .== 1)

# for 9600    bit rate, rate = 3.7385587211550857 S/s.  No errors
# for 115200  bit rate, rate = 52.512863889476286 S/s.  No errors
# for 230400  bit rate, rate = 101.45818202134197 S/s.  No errors
# for 460,800 bit rate, rate = 223.7421277172073  S/s.  No errors
# for 640,000 bit rate, rate = 299.9097240134125  S/s.  No errors
# for 768,000 bit rate, rate = 309.8916711063754  S/s.  No errors

# for 832,000 bit rate, only 40% accurate
# for 921,600 bit rate: only 23% accurate

rows = size(M, 1)
δt = 70837/1000
rate = rows/δt

# theoretical throughput for  768,000 bits/sec

Rate = 768000 # bits/second
Sample = 16*100
SampleRate = Rate/Sample # (480 S/s)