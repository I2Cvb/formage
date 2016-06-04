from subprocess import call
from os import listdir, makedirs
from os.path import isfile, join, isdir
import errno

# To ensure directories are there
def mkdir_p(path):
    try:
        makedirs(path)
    except OSError as exc:  # Python >2.5
        if exc.errno == errno.EEXIST and isdir(path):
            pass
        else:
            raise

def getImageId(s):
    return s[-7:]

def isImage(path, name):
    return isfile(join(path,name)) and ( name[-3:] == 'png')

def getFirstSubdirectory(path):
    return join(path, listdir(path)[0])

def chunk(my_list, n):
    for ii in xrange(0, len(my_list), n):
        yield my_list[ii:ii+n]

def getImages(path, subpath, images):
    full_path = map(lambda x: join(path, subpath, x), images)
    return filter(lambda p: isfile(p), full_path)

def uniqueElements(l):
    return list(set(l))

def convert(input_files, output):
    instruction = 'convert {0} -resize 50% -border 1x1 -append -rotate 90 {1}'.format( ' '.join(input_files), output )
    call(instruction, shell=True)

def convert2(input_files, output):
    instruction = "convert {0} label:'xxxxx' -resize 50% -border 1x1 -append -rotate 90 {1}".format( ' '.join(input_files), output )
    call(instruction, shell=True)


def main():

    # working directories
    proj_path_output = '/tmp/work/xx/binarize'
    proj_python_tmp = '/tmp/mosaic_tmp/'
    proj_python_output = '/tmp/mosaic/'
    proj_python_output_by_image = '/tmp/mosaic/img/'

    # Ensure they exist
    mkdir_p(proj_python_tmp)
    mkdir_p(proj_python_output)
    mkdir_p(proj_python_output_by_image)

    # Get image names to loop with
    my_path = getFirstSubdirectory(proj_path_output)
    image_ids = [ getImageId(f) for f in listdir(my_path) if isImage(my_path,f) ]
    image_ids = sorted(uniqueElements(image_ids))
    
    print "searching images in:"
    print my_path
    print "images found"
    print image_ids

    # Generate the mosaic of each output directory
    for d in listdir(proj_path_output):
        print "I'm processing directory '{0}'".format(d);
        image_path = getImages(proj_path_output, d, image_ids)

        mkdir_p(proj_python_tmp)
        out_dir = join(proj_python_tmp,d)
        mkdir_p(out_dir)

        i=0
        for i, c in enumerate(chunk(image_path, 5)):
            output_file = join(out_dir,str(i)+'.png')
            convert(c, output_file)

        mosaic_parts = map(lambda x: join(out_dir,'{0}.png'.format(x+1)), xrange(i))
        convert(mosaic_parts, join(proj_python_output,d+'.png'))

    # Generate the mosaic of each image
    for id in image_ids:
        print "I'm processing image '{0}'".format(id);
        image_path = map(lambda x: join(proj_path_output,x,id), listdir(proj_path_output))
        image_path = filter(lambda p: isfile(p), image_path)

        convert(image_path, join(proj_python_output_by_image, id))

    print map(lambda x: join(proj_path_output,x,'000.png'), listdir(proj_path_output))

if __name__ == "__main__":
    main()


