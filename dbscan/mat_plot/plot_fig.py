import numpy as np
import matplotlib.pyplot as plt
 
plt.figure(1) 
plt.figure(2) 
ax1 = plt.subplot(211)
ax2 = plt.subplot(212) 
 
x = np.linspace(0, 3, 100)
for i in xrange(5):
    plt.figure(1)
    plt.plot(x, np.exp(i*x/3),'r*')
    plt.sca(ax1) 
    plt.plot(x, np.sin(i*x))
    plt.sca(ax2) 
    plt.plot(x, np.cos(i*x))

plt.xlabel('x')
plt.ylabel('exp')
plt.legend('12345') 
plt.show()


plt.figure(3)
#fd = open('../DBSACN_cluster/cluster_1.data')
#try:
#	all_the_content = fd.read()
#finally:
#	fd.close

# Use numpy to load the data contained in the file
# '*.txt' into a 2-D array called data
data1 = np.loadtxt('../DBSCAN_cluster/cluster_1.txt')
data2 = np.loadtxt('../DBSCAN_cluster/cluster_2.txt')

# plot the first column as x, and second column as y
plt.plot(data1[:, 0], data1[:, 1],'ro',
		data2[:, 0], data2[:, 1],'g*',
		)

#plt.xlim(0.0, 10.)

plt.grid() # == plt.grid(True)
#plt.grid(color='b' , linewidth='0.3' ,linestyle='--')

plt.show()
