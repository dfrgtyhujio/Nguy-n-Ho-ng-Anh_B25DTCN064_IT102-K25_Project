#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct {
	int bookId;
	char title[50];
	char author[50];
	int publishYear;
	int quantity;
} Book;

typedef struct {
	int day;
	int month;
	int year;
} Date;

typedef struct {
	int borrowId;
	int bookId;
	Date borrowDate;
	Date borrowReturn;
	char borrowerName[50];
	int status;
} Borrow;

void inputInt(int *value, const char *message) {
    char number[10];
    
    do {
        printf("%s", message);
        
        fgets(number, sizeof(number), stdin);
        number[strcspn(number, "\n")] = '\0';
        
        if (number[0] == '\0') {
            printf("Khong duoc rong. Nhap lai.\n");
        } else {
        	int isDigit = 1;
			for (int i = 0; i < strlen(number); i++) {
            	if (number[i] < '0' || number[i] > '9') {
                	isDigit = 0;
                	break;
            	}
			}
			if (isDigit) {
				*value = atoi(number);
            	return;
        	} else {
        		printf("Chi duoc phep nhap so nguyen.\n");
			}
    	}
    } while(1);
}

void deleteSpace(char *str) {
    int index = 0;
    
    while (str[index] == ' ' || str[index] == '\t') {
        index++;
    }
    
    if (index == 0) {
    	return;
	}
	
    int i = 0;
    while (str[index] != '\0') {
        str[i++] = str[index++];
    }
    str[i] = '\0';
}

void inputString(char str[], int size, const char *message) {
    while (1) {
        printf("%s", message);
        
        fgets(str, size, stdin);
        str[strcspn(str, "\n")] = '\0';
        
        deleteSpace(str);
        
        if (strlen(str) == 0) {
            printf("Khong duoc rong. Nhap lai.\n");
            continue;
        }
        break;
    }
}

int isUniqueTitle(Book bookList[], int bookSize, char title[]) {
    for (int i = 0; i < bookSize; i++) {
        if (strcmp(bookList[i].title, title) == 0)
            return 0;
    }
    return 1;
}

void inputBook(Book bookList[], int index) {
    do {
        inputString(bookList[index].title, sizeof(bookList[index].title), "Nhap tieu de sach: ");

        if (!isUniqueTitle(bookList, index, bookList[index].title)) {
            printf("Tieu de sach bi trung\n");
            continue;
        }
        break;
    } while (1);
    
    inputString(bookList[index].author, sizeof(bookList[index].author), "Nhap ten tac gia: ");
    
    do {
		inputInt(&bookList[index].publishYear, "Nhap nam xuat ban: ");
		
        if (bookList[index].publishYear <= 1900) {
            printf("Nam xuat ban phai lon hon 1900\n");
        }
        
    } while (bookList[index].publishYear <= 1900);
    
    do {
        inputInt(&bookList[index].quantity, "Nhap so luong sach: ");
        
        if (bookList[index].quantity <= 0) {
            printf("So luong phai lon hon 0\n");
        }
        
    } while (bookList[index].quantity <= 0);
}

void addBook(Book bookList[], int *bookSize, int *nextId) {
	bookList[*bookSize].bookId = *nextId;
	inputBook(bookList, *bookSize);
	
	(*nextId)++;
    (*bookSize)++;
}

void updateBook(Book bookList[], int bookSize) {
	if (bookSize == 0) {
        printf("Danh sach sach rong.\n");
        return;
    }
    
    int idUpdate;
    inputInt(&idUpdate, "Nhap id sach can cap nhap: "); 
    
    for (int i = 0; i < bookSize; i++) {
        if (bookList[i].bookId == idUpdate) {
            inputBook(bookList, i);
            printf("Cap nhap thanh cong\n");
            return;
        }
    }
    printf("Khong tim thay sach\n");
}

void handlePagination(int *pageNumber, int totalPage) {
    int choicePage;
    
    printf("\n--- Dieu huong ---\n");
    
    if (*pageNumber > 1) {
        printf("1. Trang truoc\n");
    } else {
        printf("1. (Khong kha dung)\n");
    }
    
    printf("2. Thoat ve menu\n");
    
    if (*pageNumber < totalPage) {
        printf("3. Trang tiep\n");
    } else {
        printf("3. (Khong kha dung)\n");
    }
    
    while (1) {
        inputInt(&choicePage, "Nhap lua chon (1-3): ");
        
        if (choicePage < 1 || choicePage > 3) {
            printf("Lua chon khong hop le.\n");
            continue;
        }
        break;
    }
    
    if (choicePage == 1) {
        if (*pageNumber > 1) {
            (*pageNumber)--;
        }
    }
    else if (choicePage == 2) {
        *pageNumber = -1;
        return;
    }
    else if (choicePage == 3) {
        if (*pageNumber < totalPage) {
            (*pageNumber)++;
        }
    }
}

void showBooksPaginate(Book bookList[], int bookSize) {
    int pageNumber = 1;
    int pageSize = 10;
    int totalPage = (bookSize % pageSize == 0) ? (bookSize / pageSize) : (bookSize / pageSize + 1);
    
    if (bookSize == 0) {
        printf("Danh sach sach rong.\n");
        return;
    }
    
    do {
        printf("\nTrang %d / %d\n", pageNumber, totalPage);
		printf("+-----+--------------------------------+----------------------+------------+-------+\n");
		printf("|%-5s|%-32s|%-22s|%-12s|%-7s|\n", "ID", "Ten Sach", "Tac Gia", "Nam XB", "SL");
		printf("+-----+--------------------------------+----------------------+------------+-------+\n");
		
		int start = (pageNumber - 1) * pageSize;
		int end = start + pageSize;
		
		for (int i = start; i < end && i < bookSize; i++) {
			printf("|%-5d|%-32s|%-22s|%-12d|%-7d|\n", bookList[i].bookId, bookList[i].title, bookList[i].author, bookList[i].publishYear, bookList[i].quantity);
		}
		
		printf("+-----+--------------------------------+----------------------+------------+-------+\n");
		
        handlePagination(&pageNumber, totalPage);
        
    	if (pageNumber == -1) {
        	return;
    	}
        
    } while (1);
}

void deleteBook(Book bookList[], int *bookSize, Borrow borrowList[], int borrowSize) {
    if (*bookSize == 0) {
        printf("Danh sach sach rong.\n");
        return;
    }
    
    int idDelete;
    int index;
    int canDelete;
    
    while (1) {
        inputInt(&idDelete, "Nhap id sach can xoa: ");
        
        index = -1;
        for (int i = 0; i < *bookSize; i++) {
            if (bookList[i].bookId == idDelete) {
                index = i;
                break;
            }
        }
        
        if (index == -1) {
            printf("Khong tim thay sach. Nhap lai.\n");
            continue;
        }
        
        canDelete = 1;
        for (int i = 0; i < borrowSize; i++) {
            if (borrowList[i].bookId == idDelete && borrowList[i].status == 1) {
                canDelete = 0;
                break;
            }
        }
        
        if (!canDelete) {
            printf("Khong the xoa sach vi co nguoi dang muon chua tra. Nhap lai.\n");
            continue;
        }
        
        for (int i = index; i < *bookSize - 1; i++) {
            bookList[i] = bookList[i + 1];
        }
        (*bookSize)--;
        printf("Xoa thong tin sach thanh cong.\n");
        break;
    }
}

void toLower(char *str) {
    for (int i = 0; i < strlen(str); i++) {
        str[i] = tolower(str[i]);
    }
}

void searchBook(Book bookList[], int bookSize) {
    char keyword[50];
    int found;
    
    do {
        found = 0;
        inputString(keyword, sizeof(keyword), "Nhap ten sach can tim: ");
        
        char keywordLower[50];
        strcpy(keywordLower, keyword);
        toLower(keywordLower);
        
        for (int i = 0; i < bookSize; i++) {
            char titleLower[100];
            strcpy(titleLower, bookList[i].title);
            toLower(titleLower);
            
            if (strstr(titleLower, keywordLower) != NULL) {
                printf("%-5d | %-30s | %-20s | %-8d | %-5d\n", bookList[i].bookId, bookList[i].title, bookList[i].author, bookList[i].publishYear, bookList[i].quantity);
                found = 1;
            }
        }

        if (!found) {
            printf("Khong tim thay sach phu hop. Nhap lai.\n\n");
        }
    } while (!found);
}

int isValidDate(int day, int month, int year) {
    if (year < 1 || month < 1 || month > 12 || day < 1) return 0;
    
    int maxDays;
    switch (month) {
        case 2:
            maxDays = ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) ? 29 : 28;
            break;
        case 4: case 6: case 9: case 11:
            maxDays = 30;
            break;
        default:
            maxDays = 31;
    }
    
    return day <= maxDays;
}

int isAfterDate(Date a, Date b) {
    if (a.year > b.year) {
    	return 1;
	} else if (a.year == b.year && a.month > b.month) {
		return 1;
	} else if (a.year == b.year && a.month == b.month && a.day > b.day) {
		return 1;
	}
	return 0;
}

void inputDate(Borrow borrowList[], int index, const char message[], int allowEmpty, int isReturnDate) {
    int day, month, year;
    
    printf("%s\n", message);
    
    while (1) {
        while (1) {
            if (allowEmpty) {
                inputInt(&day, "  Nhap ngay: ");
                if (day == 0) {
                    borrowList[index].borrowReturn.day = borrowList[index].borrowReturn.month = borrowList[index].borrowReturn.year = 0;
                    return;
                }
            } else {
                inputInt(&day, "  Nhap ngay: ");
            }

            if (day >= 1 && day <= 31) {
            	break;
			}
            printf("Ngay phai tu 1 den 31. Nhap lai.\n");
        }
        
        while (1) {
            inputInt(&month, "  Nhap thang: ");
            if (month >= 1 && month <= 12) {
            	break;
			}
            printf("Thang phai tu 1 den 12. Nhap lai.\n");
        }
        
        while (1) {
            inputInt(&year, "  Nhap nam: ");
            if (year >= 1900) {
            	break;
			}
            printf("Nam phai lon hon 1900. Nhap lai.\n");
        }
        
        if (!isValidDate(day, month, year)) {
            printf("Ngay thang nam khong hop le. Nhap lai.\n");
            continue;
        }
        
        Date temp = {day, month, year};
        if (isReturnDate && !isAfterDate(temp, borrowList[index].borrowDate)) {
            printf("Ngay tra phai sau ngay muon. Nhap lai.\n");
            continue;
        }
        
        if (isReturnDate) {
            borrowList[index].borrowReturn = temp;
        } else {
            borrowList[index].borrowDate = temp;
        }
        
        printf("Ngay thang nam hop le.\n");
        break;
    }
}

void addBorrow(Borrow borrowList[], int *borrowSize, int *nextId, Book bookList[], int bookSize) {
    int index = *borrowSize;
    
    borrowList[index].borrowId = *nextId;
    
	int bookIndex = -1;
	
	while (1) {
        inputInt(&borrowList[index].bookId, "Nhap ma sach: ");
        
        for (int i = 0; i < bookSize; i++) {
            if (borrowList[index].bookId == bookList[i].bookId) {
                bookIndex = i;
                break;
            }
        }
        
        if (bookIndex == -1) {
            printf("Khong tim thay id sach. Nhap lai.\n");
        } else {
            break;
        }
    }
	
	if (bookList[bookIndex].quantity <= 0) {
        printf("Sach da het. Khong the muon.\n");
        return;
    }
	
	bookList[bookIndex].quantity--;
	
    inputDate(borrowList, index, "Nhap ngay muon sach:", 0, 0);
	inputDate(borrowList, index, "Nhap ngay tra sach (nhap 0 neu bo trong):", 1, 1);
	inputString(borrowList[index].borrowerName, sizeof(borrowList[index].borrowerName), "Nhap ten nguoi muon: ");
	
    borrowList[index].status = 1;
    
    (*nextId)++;
    (*borrowSize)++;
}

void returnBook(Borrow borrowList[], int borrowSize, Book bookList[], int bookSize) {
    if (borrowSize == 0) {
        printf("Danh sach phieu muon rong.\n");
        return;
    }
    
    int id;
    inputInt(&id, "Nhap id phieu muon can tra: ");
    
    int index = -1;
    for (int i = 0; i < borrowSize; i++) {
        if (borrowList[i].borrowId == id) {
            index = i;
            break;
        }
    }
    
    if (index == -1) {
        printf("Khong tim thay phieu muon.\n");
        return;
    }
    if (borrowList[index].borrowReturn.day == 0 && borrowList[index].borrowReturn.month == 0 && borrowList[index].borrowReturn.year == 0) {
    	
		inputDate(borrowList, index, "Nhap ngay tra sach:", 0, 1);

    } else {
        printf("Ngay tra da duoc nhap truoc do: %d/%d/%d\n", borrowList[index].borrowReturn.day, borrowList[index].borrowReturn.month, borrowList[index].borrowReturn.year);
    }
    
    borrowList[index].status = 0;
    
    for (int i = 0; i < bookSize; i++) {
        if (bookList[i].bookId == borrowList[index].bookId) {
            bookList[i].quantity++;
            break;
        }
    }
    
    printf("Tra sach thanh cong!\n");
}

void showBorrowPaginate(Borrow borrowList[], int borrowSize) {
    int pageNumber = 1;
    int pageSize = 10;
    int totalPage = (borrowSize % pageSize == 0) ? (borrowSize / pageSize) : (borrowSize / pageSize + 1);;
    
    if (borrowSize <= 0) {
        printf("Danh sach phieu muon trong.\n");
        return;
    }
    
    do {
		printf("\nTrang %d / %d\n", pageNumber, totalPage);
		printf("+--------+---------+----------------------+------------+-------------------+-------------+\n");
		printf("|%-8s|%-9s|%-22s|%-12s|%-19s|%-13s|\n", "ID phieu", "ID sach", "Nguoi muon", "Ngay muon", "Ngay tra du kien", "Trang thai");
		printf("+--------+---------+----------------------+------------+-------------------+-------------+\n");
		
		int start = (pageNumber - 1) * pageSize;
		int end = start + pageSize;
		
		for (int i = start; i < end && i < borrowSize; i++) {
			printf("|%-8d|%-9d|%-22s|", borrowList[i].borrowId, borrowList[i].bookId, borrowList[i].borrowerName);
			printf("%02d/%02d/%04d  |", borrowList[i].borrowDate.day, borrowList[i].borrowDate.month, borrowList[i].borrowDate.year);
			printf("%02d/%02d/%04d         |", borrowList[i].borrowReturn.day, borrowList[i].borrowReturn.month, borrowList[i].borrowReturn.year);
			printf("%-13s|\n", (borrowList[i].status == 0 ? "Da tra" : "Dang muon"));
		}
		
		printf("+--------+---------+----------------------+------------+-------------------+-------------+\n");
        
        handlePagination(&pageNumber, totalPage);
        
    	if (pageNumber == -1) {
        	return;
    	}
    	
    } while (1);
}


int main(){
	Book bookList[100];
	Date d[100];
	Borrow borrowList[100]; 
    int bookSize = 0, bookId = 1, borrowSize = 0, borrowId = 1;
    int choice, choiceBook, choiceBorrow;
    
	Book samples[15] = {
		{0, "Tat Den", "Ngo Tat To", 1939, 12},
		{0, "Chi Pheo", "Nam Cao", 1941, 10},
		{0, "Lao Hac", "Nam Cao", 1943, 8},
		{0, "So Do", "Vu Trong Phung", 1936, 7},
		{0, "Doi Thua", "Vu Trong Phung", 1937, 11},
		{0, "Tuoi Tho Du Doi", "Phung Quan", 1988, 9},
		{0, "Dat Rung Phuong Nam", "Doan Gioi", 1957, 6},
		{0, "Nguoi Me Cam Sung", "Nguyen Thi", 1965, 14},
		{0, "Canh Dong Bat Tan", "Nguyen Ngoc Tu", 2005, 13},
		{0, "Toi Thay Hoa Vang", "Nguyen Nhat Anh", 2010, 20},
		{0, "Mat Biec", "Nguyen Nhat Anh", 1990, 18},
		{0, "Cho Toi Xin Mot Ve", "Nguyen Nhat Anh", 2006, 15},
		{0, "Buoc Chan Lang Thang", "Nguyen Nhat Anh", 1986, 9},
		{0, "Nhat Ky Trong Tu", "Ho Chi Minh", 1943, 7},
		{0, "Vo Nhat", "Kim Lan", 1945, 12}
	};
	
	for (int i = 0; i < 15; i++) { 
    	samples[i].bookId = bookId;
    	bookId++;
    	bookList[bookSize] = samples[i];
    	bookSize++;
    }
    
    do {
    	printf("\n+-------------------------------------+\n");
        printf("|  ============= MENU ==============  |\n");
        printf("|  1. Quan li sach                    |\n");
        printf("|  2. Quan li phieu muon              |\n");
        printf("|  3. Thoat                           |\n");
        printf("+-------------------------------------+\n");
        inputInt(&choice, "Nhap lua chon: "); 
        
        switch (choice) {
            case 1:
            	do {
            		printf("\n+-------------------------------------+\n");
	            	printf("|  =========== MENU SACH ===========  |\n");
	        		printf("|  1. Them sach moi                   |\n");
	        		printf("|  2. Cap nhap thong tin sach         |\n");
	        		printf("|  3. Hien thi danh sach              |\n");
	        		printf("|  4. Xoa thong tin sach              |\n");
	        		printf("|  5. Tim kiem sach                   |\n");
	        		printf("|  6. Quay lai menu chinh             |\n");
	        		printf("+-------------------------------------+\n");
	        		inputInt(&choiceBook, "Nhap lua chon: "); 
	        		
	        		switch (choiceBook) {
	        			case 1:
	        				addBook(bookList, &bookSize, &bookId);
	        				break;
	        				
	        			case 2:
	        				updateBook(bookList, bookSize);
	        				break;
	        				
	        			case 3:
	        				showBooksPaginate(bookList, bookSize);
	        				break;
	        				
	        			case 4:
	        				deleteBook(bookList, &bookSize, borrowList, borrowSize);
	        				break;
	        				
	        			case 5:
	        				searchBook(bookList, bookSize);
	        				break;
	        				
						case 6:
							printf("Da quay lai menu chinh.\n");
				    		break;
				    		
				    	default:
				    		printf("Lua chon khong hop le.\n");
	        		}
	        		
            	} while(choiceBook != 6);
                break;
                
            case 2:
            	do {
            		printf("\n+-------------------------------------+\n");
	            	printf("|  ======== MENU PHIEU MUON ========  |\n");
	        		printf("|  1. Them moi phieu muon             |\n");
	        		printf("|  2. Tra sach                        |\n");
	        		printf("|  3. Hien thi danh sach phieu muon   |\n");
	        		printf("|  4. Quay lai menu chinh             |\n");
	        		printf("+-------------------------------------+\n");
	        		inputInt(&choiceBorrow, "Nhap lua chon: "); 
	        		
	        		switch (choiceBorrow) {
	        			case 1:
	        				addBorrow(borrowList, &borrowSize, &borrowId, bookList, bookSize);
	        				break;
	        				
	        			case 2:
	        				returnBook(borrowList, borrowSize, bookList, bookSize);
	        				break;
	        				
	        			case 3:
	        				showBorrowPaginate(borrowList, borrowSize);
	        				break;
	        				
						case 4:
							printf("Da quay lai menu chinh.\n");
				    		break;
				    		
				    	default:
				    		printf("Lua chon khong hop le.\n");
	        		}
	        		
            	} while(choiceBorrow != 4);
                break;
                
            case 3:
            	printf("Thoat chuong trinh.\n");
                break;
                
            default:
                printf("Lua chon khong hop le.\n");
        }
    } while(choice != 3);
    
    return 0; 
}

