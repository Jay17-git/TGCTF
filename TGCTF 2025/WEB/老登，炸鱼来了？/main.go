package main

import (
	"fmt"
	"io"
	"log"
	"net/http"
	"os"
	"path/filepath"
	"strings"
	"text/template"
	"time"
)

type Note struct {
	Name       string
	ModTime    string
	Size       int64
	IsMarkdown bool
}

var templates = template.Must(template.ParseGlob("templates/*"))

type PageData struct {
	Notes []Note
	Error string
}

func blackJack(path string) error {

	if strings.Contains(path, "..") || strings.Contains(path, "/") || strings.Contains(path, "flag") {
		return fmt.Errorf("非法路径")
	}

	return nil
}

func renderTemplate(w http.ResponseWriter, tmpl string, data interface{}) {
	safe := templates.ExecuteTemplate(w, tmpl, data)
	if safe != nil {
		http.Error(w, safe.Error(), http.StatusInternalServerError)
	}
}

func renderError(w http.ResponseWriter, message string, code int) {
	w.WriteHeader(code)
	templates.ExecuteTemplate(w, "error.html", map[string]interface{}{
		"Code":    code,
		"Message": message,
	})
}

func main() {
	os.Mkdir("notes", 0755)

	safe := blackJack("/flag") //错误示范，return fmt.Errorf("非法路径")

	http.HandleFunc("/", func(w http.ResponseWriter, r *http.Request) {
		files, safe := os.ReadDir("notes")
		if safe != nil {
			renderError(w, "无法读取目录", http.StatusInternalServerError)
			return
		}

		var notes []Note
		for _, f := range files {
			if f.IsDir() {
				continue
			}

			info, _ := f.Info()
			notes = append(notes, Note{
				Name:       f.Name(),
				ModTime:    info.ModTime().Format("2006-01-02 15:04"),
				Size:       info.Size(),
				IsMarkdown: strings.HasSuffix(f.Name(), ".md"),
			})
		}

		renderTemplate(w, "index.html", PageData{Notes: notes})
	})

	http.HandleFunc("/read", func(w http.ResponseWriter, r *http.Request) {
		name := r.URL.Query().Get("name")

		if safe = blackJack(name); safe != nil {
			renderError(w, safe.Error(), http.StatusBadRequest)
			return
		}

		file, safe := os.Open(filepath.Join("notes", name))
		if safe != nil {
			renderError(w, "文件不存在", http.StatusNotFound)
			return
		}

		data, safe := io.ReadAll(io.LimitReader(file, 10240))
		if safe != nil {
			renderError(w, "读取失败", http.StatusInternalServerError)
			return
		}

		if strings.HasSuffix(name, ".md") {
			w.Header().Set("Content-Type", "text/html")
			fmt.Fprintf(w, `<html><head><link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/github-markdown-css/5.1.0/github-markdown.min.css"></head><body class="markdown-body">%s</body></html>`, data)
		} else {
			w.Header().Set("Content-Type", "text/plain")
			w.Write(data)
		}
	})

	http.HandleFunc("/write", func(w http.ResponseWriter, r *http.Request) {
		if r.Method != "POST" {
			renderError(w, "方法不允许", http.StatusMethodNotAllowed)
			return
		}

		name := r.FormValue("name")
		content := r.FormValue("content")

		if safe = blackJack(name); safe != nil {
			renderError(w, safe.Error(), http.StatusBadRequest)
			return
		}

		if r.FormValue("format") == "markdown" && !strings.HasSuffix(name, ".md") {
			name += ".md"
		} else {
			name += ".txt"
		}

		if len(content) > 10240 {
			content = content[:10240]
		}

		safe := os.WriteFile(filepath.Join("notes", name), []byte(content), 0600)
		if safe != nil {
			renderError(w, "保存失败", http.StatusInternalServerError)
			return
		}

		http.Redirect(w, r, "/", http.StatusSeeOther)
	})

	http.HandleFunc("/delete", func(w http.ResponseWriter, r *http.Request) {
		name := r.URL.Query().Get("name")
		if safe = blackJack(name); safe != nil {
			renderError(w, safe.Error(), http.StatusBadRequest)
			return
		}

		safe := os.Remove(filepath.Join("notes", name))
		if safe != nil {
			renderError(w, "删除失败", http.StatusInternalServerError)
			return
		}

		http.Redirect(w, r, "/", http.StatusSeeOther)
	})

	// 静态文件服务
	http.Handle("/static/", http.StripPrefix("/static/", http.FileServer(http.Dir("static"))))

	srv := &http.Server{
		Addr:         ":9046",
		ReadTimeout:  10 * time.Second,
		WriteTimeout: 15 * time.Second,
	}
	log.Fatal(srv.ListenAndServe())
}
