import numpy as np
import matplotlib.pyplot as pl

#fd = open('../DBSACN_cluster/cluster_1.data')
#try:
#	all_the_content = fd.read()
#finally:
#	fd.close

# Use numpy to load the data contained in the file
# '*.txt' into a 2-D array called data
data1 = np.loadtxt('../DBSCAN_cluster/cluster_1.txt')
data2 = np.loadtxt('../DBSCAN_cluster/cluster_2.txt')
data3 = np.loadtxt('../DBSCAN_cluster/cluster_3.txt')
data4 = np.loadtxt('../DBSCAN_cluster/cluster_4.txt')
data5 = np.loadtxt('../DBSCAN_cluster/cluster_5.txt')
data6 = np.loadtxt('../DBSCAN_cluster/cluster_6.txt')

# plot the first column as x, and second column as y
pl.plot(data1[:, 0], data1[:, 1],'ro',
		data2[:, 0], data2[:, 1],'go',
		data3[:, 0], data3[:, 1],'bo',
		data4[:, 0], data4[:, 1],'r*',
		data5[:, 0], data5[:, 1],'g*',
		data6[:, 0], data6[:, 1],'b*',)

#pl.xlim(0.0, 10.)

pl.grid() # == plt.grid(True)
#pl.grid(color='b' , linewidth='0.3' ,linestyle='--')

pl.show()



