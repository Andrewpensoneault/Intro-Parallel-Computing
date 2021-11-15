#$ -e $JOB_ID.errors
#$ -o $JOB_ID.output
#$ -q UI
#$ -pe smp 1
#$ -cwd

matlab -nodesktop -nosplash -r 'helloworld'
