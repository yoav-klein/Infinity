#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "ext2_fs.h"
#include <string.h>

#define BLOCK_LEN (1024)
#define BASE_OFFSET (1024)
#define BLOCK_OFFSET(block) (BASE_OFFSET + (block - 1) * BLOCK_LEN)

#define PATH "/dev/ram0"


void SuperBlock(int fd)
{
	unsigned int block_size = 1024;
	struct ext2_super_block super = { 0 };
	
	lseek(fd, BASE_OFFSET, SEEK_SET);
	read(fd, &super, sizeof(super));
	
	if (super.s_magic != EXT2_SUPER_MAGIC)
	{
		printf("Here");
		exit(1); /* bad file system */	
	}
	
	
	
	printf("Reading super-block from device: FD_DEVICE :\n"
	       "Inodes count            : %u\n"
	       "Blocks count            : %u\n"
	       "Reserved blocks count   : %u\n"
	       "Free blocks count       : %u\n"
	       "Free inodes count       : %u\n"
	       "First data block        : %u\n"
	       "Block size              : %u\n"
	       "Blocks per group        : %u\n"
	       "Inodes per group        : %u\n"
	       "Creator OS              : %u\n"
	       "First non-reserved inode: %u\n"
	       "Size of inode structure : %hu\n"
	       ,
	       super.s_inodes_count,  
	       super.s_blocks_count,
	       super.s_r_blocks_count,     /* reserved blocks count */
	       super.s_free_blocks_count,
	       super.s_free_inodes_count,
	       super.s_first_data_block,
	       block_size,
	       super.s_blocks_per_group,
	       super.s_inodes_per_group,
	       super.s_creator_os,
	       super.s_first_ino,          /* first non-reserved inode */
	       super.s_inode_size);
}

struct ext2_group_desc GroupDescriptor(int fd)
{
	unsigned int block_size = 1024;
	struct ext2_group_desc group;
	
	lseek(fd, BASE_OFFSET + block_size, SEEK_SET);
	read(fd, &group, sizeof(group));
	
	printf("Reading first group-descriptor from device FD_DEVICE :\n"
	       "Blocks bitmap block: %u\n"
	       "Inodes bitmap block: %u\n"
	       "Inodes table block : %u\n"
	       "Free blocks count  : %u\n"
	       "Free inodes count  : %u\n"
	       "Directories count  : %u\n"
	       ,
	       group.bg_block_bitmap,
	       group.bg_inode_bitmap,
	       group.bg_inode_table,
	       group.bg_free_blocks_count,
	       group.bg_free_inodes_count,
	       group.bg_used_dirs_count);    /* directories count */
	
	return group;
}

void ReadInode(int fd, int inode_no, const struct ext2_group_desc *group, 
														struct ext2_inode* inode)
{
	lseek(fd, BLOCK_OFFSET(group->bg_inode_table)+(inode_no-1)*sizeof(struct ext2_inode), SEEK_SET);
	read(fd, inode, sizeof(struct ext2_inode));
}

void PrintInode(int fd, const struct ext2_group_desc *group)
{
	struct ext2_inode inode;
	
	/*ReadInode(fd, 2, group, &inode);
	*/
	lseek(fd, BLOCK_OFFSET(258) + sizeof(struct ext2_inode), SEEK_SET);
	read(fd, &inode, sizeof(struct ext2_inode));
	
	printf("Reading root inode\n"
	       "File mode: %hu\n"
	       "Owner UID: %hu\n"
	       "Size     : %u bytes\n"
	       "Blocks   : %u\n"
	       ,
	       inode.i_mode,
	       inode.i_uid,
	       inode.i_size,
	       inode.i_blocks);
	
}

void PrintEntry(struct ext2_dir_entry_2 *entry)
{
	printf("-------\n");
	printf("Entry: \n");
	printf("Inode number: %u\n", entry->inode);
	printf("Rec Len: %u\n", entry->rec_len);
	printf("Name Len: %u\n", entry->name_len);
	printf("File name: %s\n", entry->name);
	
}

void FindFile(int fd, struct ext2_group_desc *group, char *path)
{
	struct ext2_inode root_inode;
	struct ext2_inode file_inode;
	struct ext2_dir_entry_2 *entry;
	size_t num_of_first_block;
	size_t size = 0;
	size_t requested_inode;
	int i;
	char block[BLOCK_SIZE];
	char result[256];
	
	
	/* read root inode */
	ReadInode(fd, 2, group, &root_inode);
	
	num_of_first_block = root_inode.i_block[0];
	lseek(fd, BLOCK_OFFSET(num_of_first_block), SEEK_SET);
	read(fd, block, BLOCK_SIZE);
	
	entry = (struct ext2_dir_entry_2*)block;
	
	while(size < root_inode.i_size && entry->inode)
	{
		char file_name[EXT2_NAME_LEN + 1];
		memcpy(file_name, entry->name, entry->name_len);
		file_name[entry->name_len] = '\0';
		if(0 == strcmp(path, file_name))
		{
			printf("File found!\n");
			requested_inode = entry->inode;
			printf("Requested inode: %u\n", requested_inode);
		}
		entry = (char*)entry + entry->rec_len;
		size += entry->rec_len;

	}
	
	ReadInode(fd, requested_inode, group, &file_inode);
	num_of_first_block = file_inode.i_block[11];
	lseek(fd, BLOCK_OFFSET(num_of_first_block), SEEK_SET);
	read(fd, result, 255);
	result[255] = '\0';
	
	for(i = 0; i < 256; ++i)
	{
		printf("%c ", result[i]);
	}
	
	printf("first block:\n");
	printf("%s\n", result);
	
	
	
}

int main(int argc, char **argv)
{
	int fd;
	struct ext2_group_desc group;
	
	if(0 > (fd = open(PATH, O_RDONLY)))
	{
		printf("Couldn't open file");
	}
	
	SuperBlock(fd);
	
	group = GroupDescriptor(fd);
	printf("------\n");
	
	FindFile(fd, &group, argv[1]);
	PrintInode(fd, &group);
	
	
	close(fd);

	return 0;
}
